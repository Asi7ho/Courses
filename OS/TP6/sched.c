
/**
 * @file sched.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Feb  9 17:15:43 2018
 * @brief Scheduler.
 *
 * Scheduler.
 */

#include <unistd.h>    /* pid_t, getpid           */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include <sys/types.h> /* Message queue           */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>  /* open(2) flags (S_I?USR) */
#include <sys/time.h>  /* setitimer(2)            */

#include "queue.h"
#include "protocol.h"
#include "config.h"

/* Scheduler view of Processes Control Blocks.
 */
typedef struct {
  pid_t                     pid;
  ssize_t                   ttl;
  size_t                  burst;
  struct timeval last_scheduled;
  size_t         total_duration;

  /* Metrics. */
  double              cpu_usage;
  size_t          response_time;
  int               already_ran;
  struct timeval   arrival_date;
} proc;

/* Our Scheduler.
 */
enum { STATE_INIT,     /* Transient state.                                */
       STATE_IDLE,     /* Default state: pause(2)'d.                      */
       STATE_ALRM,     /* Rescheduling caused by timer interrupt.         */
       STATE_NOTIFIED, /* At least one Process message needs to be read.  */
       STATE_FINISH    /* Transient state.                                */
};

typedef struct sched_state_t {
  /* Host OS interface.                                                   */
  int                     qid; /* SYS V message queue ID.                 */
  struct itimerval      timer; /* Timer for interrupt-based scheduling.   */

  /* The scheduling policy is implemented as a priority queue and a
     function that returns the time before the next preemption.           */
  comparator           policy;

  volatile sig_atomic_t state; /* Scheduler state.                        */

  /* A Process is located either 'on_cpu', or in the Ready queue 'procs'. */
  proc                *on_cpu; /* if !NULL, currently RUNNING Process.    */
  queue                 procs; /* Process Ready queue.                    */
  proc                  *last; /* Last Process removed from CPU.          */

  /* Bookkeeping.                                                         */
  int                   nproc; /* Number of Processes in the Scheduler.   */
  struct timeval  batch_start; /* Last time a "batch" has been started.   */
  struct timeval         when; /* Last time a Process has been scheduled. */

  /* Metrics.                                                             */
  size_t           batch_size; /* Size of the "batch".                    */
  size_t       batch_duration; /* Duration of the "batch".                */
  size_t     batch_turnaround; /* Sum of turnaround times.                */
  size_t       batch_response; /* Sum of response times.                  */
} sched_state;

static sched_state scheduler;

static void *new_pcb( pid_t pid, size_t duration, size_t init_burst ) {

  proc           *n = calloc( 1, sizeof( *n ) );
  n->pid            = pid;
  n->ttl            = duration;
  n->burst          = init_burst;
  n->cpu_usage      = 0.;
  n->response_time  = 0;
  n->already_ran    = 0;
  n->total_duration = n->ttl;

  gettimeofday( &n->last_scheduled, NULL );
  n->arrival_date   = n->last_scheduled;

  return n;
}

/* Callbacks for Scheduler PCBs handling.
 */
static void update_burst( void *ptr, ssize_t newval ) {
  ((proc*)ptr)->burst = newval;
}

static void subtract_ttl( void *ptr, ssize_t duration ) {
  proc *p = ptr;

  p->ttl = duration >= p->ttl ? 0 : (p->ttl-duration);
}

static void print_pcb( void *ptr, ssize_t parm ) {
  proc  *p = ptr;

  printf( COLORED(HIGH, RED,"%d")"{"COLORED(BOLD, BLUE,"%lu")","COLORED(HIGH, YELLOW,"%lu")",%.2lf} ", p->pid, p->burst, p->ttl, p->cpu_usage );

  return;
  UNUSED( parm );
}

static void del_pcb( void *p, ssize_t parm ) {
  free( p );
  return;
  UNUSED( parm );
}

static void dont_del_pcb( void *p, ssize_t parm ) {
  return;
  UNUSED( p );
  UNUSED( parm );
}

/* To find a Process in the Ready Queue by its PID. */
static int  eq_pid( void *ptr, void *t ) {
  return ((proc*)ptr)->pid != *(pid_t*)t;
}




/* Available Scheduling policies: RR, SJF.
 */


/* RR (Round-Robin)  policy . Processes are
   re-enqueued one after the other.
 */
static int sched_rr( void *curr_ptr, void *test_ptr ) {
  return 1;
  UNUSED( curr_ptr );
  UNUSED( test_ptr );
}

/* SJF (Shortest-Job-First) policy. Processes
   are re-enqueued in decreasing 'burst' duration
   order. Also, this is a preemptive version of
   SJF, some sort of STCF if you like
   (Shortest-Time-to-Completion-First).
 */
static int sched_sjf( void *curr, void *test ) {
  return ((proc*)curr)->burst - ((proc*)test)->burst;
}

/* MLFQ (Multi-Level-Feedback-Queue) policy. Processes
   are re-enqueued in increasing 'cpu_usage' order.
 */
static int sched_mlfq( void *curr, void *test ) {
  double diff = ((proc*)curr)->cpu_usage - ((proc*)test)->cpu_usage;
  return diff < 0 ? -1 : ( diff == 0 ? 0 : 1 );
}


static size_t sched_next_slice_duration( sched_state *sched ) {
  return sched->on_cpu ?
    (sched->on_cpu->burst <= QUANTUM_DFLT ? sched->on_cpu->burst : QUANTUM_DFLT)
    : QUANTUM_DFLT;
}


/* SIGALRM is used to implement an interrupt-based preemptive
   scheduling loop.

   The only other rescheduling opportunity is caused by  a
   YIELD or DELETE message issued by the Process currently
   'on_cpu' (respectively simulating I/O operation initiation
   during the allocated time slice, or the regular end of the
   Process).
 */
static void sigalrm_handler( int signum ) {
  RELOAD_HANDLER( sigalrm_handler );
  scheduler.state = STATE_ALRM;
  return;
  UNUSED( signum );
}

/* A Process sends SIGUSR2 to the Scheduler to signal
   that (at least) one message is available for reading.
*/
static void sigusr2_handler( int signum ) {
  RELOAD_HANDLER( sigusr2_handler );
  scheduler.state = STATE_NOTIFIED;
  return;
  UNUSED( signum );
}

/* Finish simulation on SIGINT/SIGTERM.
 */
static void sigterm_handler( int signum ) {
  scheduler.state = STATE_FINISH;
  return;
  UNUSED( signum );
}

/* Run a Process, return next time slice advertised
   duration (but the Process may lie, simulating an
   unexpected I/O operation by a YIELD message).
*/
static int run_process( proc *process ) {
  int next_duration = 0;

  printf( "* Starting Process on CPU: " );
  print_pcb( process, 0 );
  printf( "\n" );

  if ( 0 == process->already_ran ) {
    struct timeval first_run;
    gettimeofday( &first_run, NULL );
    process->already_ran      = 1;
    process->response_time    = TIME_DIFF_IN_FAKE_MS( first_run, process->arrival_date );
    scheduler.batch_response += process->response_time;
  }

  CHECK_CALL( -1, kill( process->pid, SIGCONT ), "kill(2)" );
  gettimeofday( &process->last_scheduled, NULL );

  scheduler.when = process->last_scheduled;
  /* Next slice duration *may* be related to the next Process CPU burst: */
  next_duration  = sched_next_slice_duration( &scheduler );
  printf( "* "COLORED(BOLD, BLUE, "Next interrupt in %d ms")"\n", next_duration );

  return next_duration;
}

static void process_messages( int allow_resched );

/* Choose next Process to run on the CPU.

   No signal (Scheduler state change) can be taken  into
   account during execution of this code (even if any of
   them actually occur).
 */
static void reschedule( void ) {

  size_t          next_duration = 0;

  scheduler.last = scheduler.on_cpu;

  if ( scheduler.on_cpu ) {
    /* There is a Process to be STOPPED. */
    size_t          duration = 0;
    struct timeval       now;

    /* Perform *context switch* between the RUNNING Process
       and next STOPPED Process:
    */
    printf( "* Stopping Process on CPU: " );
    print_pcb( scheduler.on_cpu, 0 );
    printf( "\n" );

    /* 1. Change current Process state: RUNNING -> STOPPED
       (Kindly ask the Process to relinquish the CPU).
    */
    CHECK_CALL( -1, kill( scheduler.on_cpu->pid, SIGUSR1 ), "kill(2)" );

    if ( !emptyq( scheduler.procs ) ) {
      printf( "* Ready queue: " );
      printq( scheduler.procs, print_pcb );
      printf( "\n" );
    }

    gettimeofday( &now, NULL );
    duration = TIME_DIFF_IN_FAKE_MS( now, scheduler.on_cpu->last_scheduled );
    subtract_ttl( scheduler.on_cpu, duration );

    scheduler.on_cpu->cpu_usage = (scheduler.on_cpu->cpu_usage + (duration)/((double)scheduler.on_cpu->burst))/2;

    printf( "* "COLORED(BOLD, BLUE,"Last slice: %lu ms")".\n", duration );

    /* Avoid (spurious) rescheduling during rescheduling...
    */
    process_messages( 0 );

    /* 2. Grab next Process to be scheduled;
     */
    scheduler.on_cpu  = queue_peek( scheduler.procs );

    /* 3. Remove it from Ready Queue (will be inserted
       again after next process is selected);
    */
    scheduler.procs   = dequeue( scheduler.procs );

    if ( scheduler.on_cpu ) {
      /* 4. Next Process is STOPPED, change its state: STOPPED -> RUNNING.
       */
      next_duration   = run_process( scheduler.on_cpu );
    }
    else {
      /* Only one Process in the Scheduler, re-schedule *quickly* to select it
	 again. That's the real trick here. Ensures we will actually react  to
	 all signals, both in the Scheduler *and* in the Process (otherwise, a
	 Process state may change without being noticed  because signals would
	 be sent  too  quickly and  missed during the Process state monitoring
	 loop).
      */
      next_duration   = 1;
    }

    if ( scheduler.last ) {
      /* 5. Re-enqueue last STOPPED Process (if any) according to the Scheduler
	 policy.
      */
      scheduler.procs = enqueue( scheduler.procs, scheduler.last, scheduler.policy );
    }

  }
  else { /* Last time slice without running a Process, try to get one now. */
    if ( !emptyq( scheduler.procs ) ) {
      printf( "* Ready queue: " );
      printq( scheduler.procs, print_pcb );
      printf( "\n" );
    }

    /* 1. Try to grab next Process;
     */
    scheduler.on_cpu = queue_peek( scheduler.procs );
    scheduler.procs  = dequeue( scheduler.procs );

    if ( scheduler.on_cpu ) {
      /* 2. We have a new process, change its state: STOPPED -> RUNNING;
       */
      next_duration  = run_process( scheduler.on_cpu );
    }
    else { /* No Process in the Scheduler, resume printing a dot every quantum. */
      next_duration  = QUANTUM_DFLT;
    }
  }

  /* Set next SIGALRM timer for interrupt-caused preemption. */
  SET_TIMER_IN_FAKE_MS( scheduler.timer, next_duration );
}

static proc *find_pcb( size_t pid ) {

  if ( scheduler.on_cpu && (pid_t)pid == scheduler.on_cpu->pid ) {
    return scheduler.on_cpu;
  }
  else if ( scheduler.last && (pid_t)pid == scheduler.last->pid ) {
    return scheduler.last; /* All cases (paranoid life-style). */
  }
  else { /* Do not reschedule if the Process is in the Ready queue (already). */
    return queue_search( scheduler.procs, eq_pid, &pid );
  }

}

/* If 0 == type, process all messages.
 */
static void process_messages( int allow_resched ) {
  proc_q_msg             msg;
  struct timeval         now;
  int                resched = 0;

  gettimeofday( &now, NULL );

  /* TODO: Examine all available messages each time. Replace '0' condition below
     with a call to msgrcv(2).
  */

  while (-1 != msgrcv(scheduler.qid, (void *)&msg, PROC_Q_MSGSZ, 0, IPC_NOWAIT)) {

    proc *nproc = NULL;

    switch( msg.type ) {

    case PROC_BURST : /* A Process is sending its next burst. */
      printf( "* Next burst for "COLORED(HIGH, RED, "%d")": %lu ms.\n",
	      msg.pid, msg.next_burst );

      nproc = find_pcb( msg.pid );

      update_burst( nproc, msg.next_burst );

      if ( ( NULL == scheduler.on_cpu || ( scheduler.on_cpu && msg.pid != scheduler.on_cpu->pid ) ) &&
	   scheduler.policy == sched_sjf ) {
	/* Keep Ready queue in order when using SJF. */
	scheduler.procs = delval( scheduler.procs, dont_del_pcb, eq_pid, &msg.pid );
	scheduler.procs = enqueue( scheduler.procs, nproc, scheduler.policy );
      }
      break;

    case PROC_YIELD : /* The RUNNING Process is relinquishing the CPU.  */
      resched = 1;
      break;

    case PROC_CREATE : /* A Process is asking for creation in the Ready queue. */
      CHECK_CALL( -1, kill( msg.pid, SIGUSR1 ), "kill(2)" );
      nproc = new_pcb( msg.pid, msg.ttl, 1 );
      printf( "* Creating Process "COLORED(HIGH,RED,"%d")" for "COLORED(HIGH, YELLOW,"%lu")" ms.\n",
	      msg.pid, msg.ttl );
      if ( 0 == scheduler.nproc ) {
	gettimeofday( &scheduler.batch_start, NULL );
	scheduler.batch_size = 0;
	scheduler.batch_duration = 0;
      }
      scheduler.nproc++;
      scheduler.batch_size++;
      scheduler.batch_duration += msg.ttl;

      scheduler.procs = enqueue( scheduler.procs, nproc, scheduler.policy );
      break;

    case PROC_DELETE : /* A Process is asking for deletion. */
      nproc = find_pcb( msg.pid );

      printf( "* Terminating Process "COLORED(HIGH,RED,"%d")" (sending SIGTERM)\n",
	      msg.pid );
      printf( "\tResponse time  : %ld ms.\n", nproc->response_time );
      printf( "\tTurnaround time: %lu ms (wanted %lu ms).\n",
	      TIME_DIFF_IN_FAKE_MS( now, nproc->arrival_date ),
	      nproc->total_duration );
      CHECK_CALL( -1, kill( msg.pid, SIGTERM ), "kill(2)" );

      scheduler.batch_turnaround += TIME_DIFF_IN_FAKE_MS( now, nproc->arrival_date );
      scheduler.batch_duration   -= msg.ttl;

      scheduler.procs = delval( scheduler.procs, del_pcb, eq_pid, &msg.pid );
      if ( scheduler.on_cpu && msg.pid == scheduler.on_cpu->pid ) {
	if ( scheduler.on_cpu == scheduler.last ) scheduler.last = NULL;
	resched = 2; /* Reschedule. */
	del_pcb( scheduler.on_cpu, 0 );
	scheduler.on_cpu = NULL;
      }

      scheduler.nproc--;
      if ( 0 == scheduler.nproc ) {
	size_t total_duration;

	gettimeofday( &now, NULL );

	total_duration = TIME_DIFF_IN_FAKE_MS( now, scheduler.batch_start );

	printf( "\n* Last batch: "COLORED(HIGH, RED, "%lu Processes (%lu ms) terminated in %lu ms")"\n"
		"\t("COLORED(HIGH, RED, "%lu ms wasted")" in scheduling)\n",
		scheduler.batch_size,
		scheduler.batch_duration,
		total_duration,
		total_duration - scheduler.batch_duration );
	printf( "\tMean "COLORED(HIGH, RED, "response")" time  : "COLORED(HIGH, RED, "%.2lf ms")".\n",
		scheduler.batch_response/(double)scheduler.batch_size );
	printf( "\tMean "COLORED(HIGH, RED, "turnaround")" time: "COLORED(HIGH, RED, "%.2lf ms")".\n\n",
		scheduler.batch_turnaround/(double)scheduler.batch_size );
      }

      break;

    default : /* Should not happen. */
      fprintf( stderr, "Wrong message type (%ld)\n", msg.type );
      break;
    }
  }

  if ( allow_resched && resched ) {
    /* Disable (possibly) pending SIGALRM. */
    SET_TIMER_IN_FAKE_MS( scheduler.timer, 0 );

    if ( 1 == resched )
      printf( "* "COLORED(HIGH, GREEN,"Reschedule on YIELD!") );
    else
      printf( "* "COLORED(HIGH, GREEN,"Reschedule on DELETE!") );

    printf( " (after %lu ms)\n", TIME_DIFF_IN_FAKE_MS( now, scheduler.when ) );

    reschedule();
  }

}

/* "Shutdown" sequence. Mark SYS V message queue for deletion in
   the host OS, send all Processes in the Scheduler the  SIGTERM
   signal, and remove the Scheduler PID file.
 */
static void finish_sched( void ) {

  LOAD_HANDLER( SIGINT , SIG_IGN );
  LOAD_HANDLER( SIGTERM, SIG_IGN );
  LOAD_HANDLER( SIGALRM, SIG_IGN );

  printf( "\nPower off sequence started.\n" );

  printf( "* Sending all Processes the SIGTERM signal: " );

  if ( scheduler.on_cpu ) {
    print_pcb( scheduler.on_cpu, 0 );
    CHECK_CALL( -1, kill( scheduler.on_cpu->pid, SIGCONT ), "kill(2)" );
    CHECK_CALL( -1, kill( scheduler.on_cpu->pid, SIGTERM ), "kill(2)" );
  }

  while ( !emptyq( scheduler.procs ) ) {
    pid_t *next = queue_peek( scheduler.procs );

    print_pcb( next, 0 );
    CHECK_CALL( -1, kill( *next, SIGCONT ), "kill(2)" );
    CHECK_CALL( -1, kill( *next, SIGTERM ), "kill(2)" );

    del_pcb( next, 0 );
    scheduler.procs = dequeue( scheduler.procs );
  }
  printf( "done.\n" );

  CHECK_CALL( -1, unlink( SCHED_PID ), "unlink(2)" );

  printf( "* Deleted Scheduler PID file.\n" );
  printf( "* Marking SYS V queue for deletion.\n" );

  /* msgsnd(2) in Process DELETE may block if the SYS V message
     queue is deleted too early. Indulging ourselves a  benign,
     quick'n'dirty fix..
  */
  usleep( 1000 );

  CHECK_CALL( -1, msgctl( scheduler.qid, IPC_RMID, NULL ), "msgctl(2)" );

  printf( "System halted.\n" );
}

/* This is the file used by ftok(3). A new Process will find the
   PID of the Scheduler inside, so it can send signals to it.
 */
static void write_sched_pid( void ) {
  FILE *fp = fopen( SCHED_PID, "wt" );

  fprintf( fp, "%d\n", getpid() );

  fclose( fp );
}

/* "Boot" sequence. Create SYS V message queue in the host
   OS, set signal handlers and send the first  SIGALRM  to
   initiate the Scheduler main preemptive scheduling  loop.
 */
static void init_sched( comparator policy ) {
  key_t key;

  scheduler.policy           = policy;
  scheduler.procs            = new_queue();
  scheduler.nproc            = 0;
  scheduler.on_cpu           = NULL;
  scheduler.last             = NULL;
  scheduler.batch_turnaround = 0;

  printf( "Power on sequence started.\n" );

  write_sched_pid();
  printf( "* Wrote scheduler PID: %d.\n", getpid() );

  /* TODO: Construct IPC key from file SCHED_PID. Use ftok(3).
   */
  key = 0;

  key = ftok(SCHED_PID, 42);

  if ( -1 == key ) {
    perror( "ftok(3)" );
    exit( EXIT_FAILURE );
  }

  /* TODO: Create SYS V message queue. Use megget(2) with flags
     S_IRUSR | S_IWUSR (at least!).
   */

  scheduler.qid = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);

     if (scheduler.qid == -1) {
       printf("Erreur creation de la queue\n");
       exit(1);
     }

  printf( "* Process queue created.\n" );

  LOAD_HANDLER( SIGINT , sigterm_handler );
  LOAD_HANDLER( SIGTERM, sigterm_handler );
  LOAD_HANDLER( SIGUSR2, sigusr2_handler );
  LOAD_HANDLER( SIGALRM, sigalrm_handler );

  printf( "Awaiting new Processes\n" );

  gettimeofday( &scheduler.when, NULL );

  /* The trick is to keep breathing.. (tribute to Shirley Manson).

     This is intended as a default failcase to ensure scheduling
     will happen, regardless of any (possible) scheduling policy
     implementation mistakes..
   */
  scheduler.timer.it_interval.tv_sec  = ((int)(QUANTUM_DFLT*DURATION_SCALE*1000))/(int)1e6;
  scheduler.timer.it_interval.tv_usec = ((int)(QUANTUM_DFLT*DURATION_SCALE*1000))%(int)1e6;

  /* Going live in 3, 2, *1* fake ms, Baby! */
  SET_TIMER_IN_FAKE_MS( scheduler.timer, 1 );

  return;
}

static void print_usage( char *prog ) {
  fprintf( stderr, "Usage: %s policy\n", prog );
  fprintf( stderr, "  Where policy is: \n" );
  fprintf( stderr, "    rr     : Round-Robin\n" );
  fprintf( stderr, "    sjf    : Shortest-Job-First\n" );
  fprintf( stderr, "    mlfq   : Multi-Level Feedback Queue\n" );
  exit( EXIT_FAILURE );
}

int main ( int argc, char *argv[] ) {

  if ( 1 == argc ) print_usage( argv[ 0 ] );

  if ( !strcmp( argv[ 1 ], "rr" ) ) {
    init_sched( sched_rr );
  }
  else if ( !strcmp( argv[ 1 ], "sjf" ) ) {
    init_sched( sched_sjf );
  }
  else if ( !strcmp( argv[ 1 ], "mlfq" ) ) {
    init_sched( sched_mlfq );
  }
  else print_usage( argv[ 0 ] );

  /* The Scheduler only reacts to *signals*. Preemption is
     implemented by means of intercepting SIGALRM.
   */
  while( STATE_FINISH != scheduler.state ) {
    switch( scheduler.state ) {
    case STATE_NOTIFIED :
      process_messages( 1 );
      scheduler.state = STATE_IDLE;
      break;
    case STATE_ALRM :
      if ( scheduler.nproc ) {
	printf( "* "COLORED(HIGH, GREEN,"Reschedule on interrupt!")"\n" );
      }
      else {
	printf( COLORED(HIGH, GREEN, ".") ); fflush( stdout );
      }
      reschedule();
      scheduler.state = STATE_IDLE;
      break;
    case STATE_INIT :
    case STATE_IDLE :
      pause();
      break;
    case STATE_FINISH :
      break;
    }
  }

  finish_sched();

#ifdef DEBUG
  printf( "Press enter to continue..." );
  getchar();
#endif
  exit( EXIT_SUCCESS );

  UNUSED( argc );
  UNUSED( argv );
}
