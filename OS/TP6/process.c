
/**
 * @file proc.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Feb  9 17:16:38 2018
 * @brief Process.
 *
 * Process.
 */


#include <unistd.h>    /* pid_t, getpid(2) */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <sys/types.h> /* Message queue    */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/time.h>  /* setitimer(2)     */

#include "config.h"
#include "protocol.h"

static pid_t read_sched_pid( void ) {
  FILE   *fp = fopen( SCHED_PID, "rt" );
  pid_t  pid = -1;

  if ( NULL == fp ) {
    perror( "fopen(3)" );
    return -1;
  }

  fscanf( fp, "%d", &pid );
  fclose( fp );

  return pid;
}


/* This is our skeleton to model a Process
   Control Block.
 */
struct proc_state {
  volatile sig_atomic_t    state; /* Process state.                                       */
  pid_t                      pid; /* Processes are known by their PID.                    */
  ssize_t                    ttl; /* Process remaining Time To Live.                      */

  /* Scheduler info. */
  pid_t                sched_pid; /* PID of the Scheduler to send signals to it.          */
  int                     proc_q; /* ID of the SYS V msg queue to talk to the Scheduler.  */
  int                 next_burst; /* Next burst, as *advertised* (only) to the Scheduler. */
  int              last_duration; /* Last actual execution duration (measured).           */

  /* Process personality. */
  int                  max_burst; /* To simulate Processes with different behaviours.     */
  int                 bound_type; /* Lower is I/O-bound Process, higher is CPU-bound.     */
};

enum { STATE_CREATE,
       STATE_RUNNING,
       STATE_STOPPED,
       STATE_YIELD,
       STATE_INTERRUPT,
       STATE_DELETE
};

/* PCB for this Process. */
static struct proc_state pcb;


static void sigcont_handler( int signum ) {
  RELOAD_HANDLER( sigcont_handler );
  pcb.state = STATE_RUNNING;
  return;
}

static void sigalrm_handler( int signum ) {
  RELOAD_HANDLER( sigalrm_handler );
  pcb.state = STATE_YIELD;
  return;
}

static void sigusr1_handler( int signum ) {
  RELOAD_HANDLER( sigusr1_handler );
  pcb.state = STATE_STOPPED;
  return;
}

/* SIGTERM is used to terminate the POSIX process.

   Upon reception , print the event and (#ifdef DEBUG)
   give a chance to manually check the process output.
   Finally, call exit(3). We could return a PCB final
   value to the system, but we could not care less.
 */
static void sigterm_handler( int signum ) {
  pcb.state = STATE_DELETE;
  return;
  UNUSED( signum );
}

/* SIGINT is used to terminate a Process properly when
   the user hits Ctrl-C.
 */
static void sigint_handler( int signum ) {

  pcb.state = STATE_INTERRUPT;

  return;
  UNUSED( signum );
}

static int notify_sched( int msgtype, int next_burst, int ttl, int last_duration ) {
  proc_q_msg msg;

  msg.type          = msgtype;
  msg.pid           = pcb.pid;
  msg.ttl           = ttl;
  msg.last_duration = last_duration;
  msg.next_burst    = next_burst;

  /* TODO: Send message to SYS V queue. Use msgsnd(2).
   */
   if (msgsnd(pcb.proc_q, (void *)&msg, PROC_Q_MSGSZ, 0) != 0) {
     printf("Message non envoyé\n");
     exit(1);
   } else {
     printf("message envoyé\n");
   }


  /* Send  SIGUSR2  to signal the scheduler that the SYS V
     queue has a new message to read (at least , since any
     other process may have sent a message in the queue in
     the meantime).
  */
  kill( pcb.sched_pid, SIGUSR2 );

  return 0;
}

/* This is the glue to serve as the Process "prologue"
   for the POSIX process. Signal handlers are set up,
   and a creation request is sent for the Scheduler
   into the SYS V message queue. Returning a Process
   in transient STATE_CREATE state (expecting SIGSTOP
   very shortly after).
 */
static int init_proc( size_t duration, int max_burst, int process_type ) {

  /* TODO: Construct IPC key from file SCHED_PID. Use ftok(3).
   */
  key_t key = 0;

  key = ftok(SCHED_PID, 42);

  LOAD_HANDLER( SIGINT , sigint_handler  );
  LOAD_HANDLER( SIGTERM, sigterm_handler );
  LOAD_HANDLER( SIGUSR1, sigusr1_handler );
  LOAD_HANDLER( SIGALRM, sigalrm_handler );
  LOAD_HANDLER( SIGCONT, sigcont_handler );

  pcb.pid              = getpid();
  pcb.ttl              = duration;
  pcb.state            = STATE_CREATE;
  pcb.max_burst        = max_burst;
  pcb.sched_pid        = read_sched_pid();
  pcb.bound_type       = process_type;
  pcb.next_burst       = 1+random()%pcb.max_burst;

  if ( -1 == pcb.sched_pid ) {
    fprintf( stderr, "Scheduler not running, aborting.\n" );
    return -1;
  }

  if ( -1 == key ) {
    perror( "ftok(3)" );
    return -1;
  }

  /* TODO: Connect to SYS V message queue. Use msgget(2).
   */

  pcb.proc_q = msgget(key, 0);

  if ( -1 == pcb.proc_q ) {
    perror( "msgget(2)" );
    return -1;
  }

  fprintf( stderr, "\nNew process "COLORED(HIGH,RED,"%d")" with duration "COLORED(HIGH,YELLOW,"%lu")" ms.\n\n", pcb.pid, pcb.ttl );
  srandom( pcb.pid );

  notify_sched( PROC_CREATE, pcb.next_burst, pcb.ttl, 0 );

  return 0;
}

static void set_next_alrm( int actual_burst ) {
  struct itimerval relinquish_time;

  relinquish_time.it_interval.tv_sec  = 0;
  relinquish_time.it_interval.tv_usec = 0;

  /* Clear (possibly) pending SIGALRM delivery. */
  RESET_TIMER_IN_FAKE_MS( relinquish_time );
  /* Set next SIGALRM delivery. */
  SET_TIMER_IN_FAKE_MS( relinquish_time, actual_burst );
}

static void reset_alrm( void ) {
  set_next_alrm( 0 );
}

/* The art of lying to the Scheduler (simulating I/O).
 */
static void set_next_actual_burst( int process_type ) {

  /* If process_type is high , the Process is I/O-bound.
     Otherwise it is CPU-bound (it will often relinquish
     the CPU despite its advertised 'next_burst' to  the
     Scheduler).
  */
  if ( random()%process_type && 3 < pcb.next_burst ) {
    int actual_burst = 2+random()%(pcb.next_burst-2);
    /* Ensures a minimal duration of 2 ms. */
    fprintf( stderr, "\nNext SIGALRM for YIELD in "COLORED(BOLD,GREEN,"%d")" ms.", actual_burst );
    set_next_alrm( actual_burst );
  }
}




static int save_the_planet( void ) {
  struct timespec one_fake_ms;
  one_fake_ms.tv_sec     = ((int)(DURATION_SCALE*1000*1000))/(int)1e9;
  one_fake_ms.tv_nsec    = ((int)(DURATION_SCALE*1000*1000))%(int)1e9;

  /* Don't suck too much on the (real) CPU, save the planet!
   */
  return nanosleep( &one_fake_ms, NULL );
}




static int do_something_useful( void ) {

  return save_the_planet();
}




static void print_usage( char *prog ) {
  fprintf( stderr, "\n  Usage: %s duration max_burst type\n", prog );
  fprintf( stderr, "\n  Where: \n" );
  fprintf( stderr, "      duration   is the Process duration (ms).\n" );
  fprintf( stderr, "      max_burst  is the Process max burst duration (ms).\n" );
  fprintf( stderr, "      type       is the Process type (lower: CPU, higher: I/O).\n\n" );
  exit( EXIT_FAILURE );
}

int main ( int argc, char *argv[] ) {

  int   stopped = 0;
  int   running = 0;
  int     yield = 0;

  if ( argc != 4 ) print_usage( argv[ 0 ] );

  if ( -1 == init_proc( atoi( argv[ 1 ] ), atoi( argv[ 2 ] ) - 1, atoi( argv[ 3 ] ) ) ) {
    fprintf( stderr, "Aborting launch of Process %d.\n", pcb.pid );
    pcb.state = STATE_DELETE;
  }

  while ( STATE_DELETE != pcb.state ) {
    switch( pcb.state ) {
    case STATE_CREATE :
      /* Busy wait for SIGSTOP. Not an issue because transient state. */
      break;
    case STATE_RUNNING :
      if ( 0 == running ) {
	stopped = 0;
	yield   = 0;
	running = 1;
	pcb.last_duration = 0;

	set_next_actual_burst( pcb.bound_type );

	fprintf( stderr, "\nRunning " );
      }

      /* We have the CPU, let's do some stuff. */
      do_something_useful();
      fprintf( stderr, COLORED(BOLD,BLUE,".") );
      pcb.last_duration++;

      if ( pcb.ttl == pcb.last_duration ) {
	pcb.ttl = 0;
	raise( SIGINT );
      }

      break;
    case STATE_YIELD :
      if ( 0 == yield ) {
	yield = 1;
	fprintf( stderr, "\n"COLORED(HIGH,GREEN,"Relinquish the CPU!") );

	notify_sched( PROC_YIELD, pcb.next_burst, pcb.ttl, pcb.last_duration );
      }
      /* Busy wait for SIGUSR1. Transient state. */
      break;
    case STATE_STOPPED :
      if ( 0 == stopped ) {
	running          = 0;
	yield            = 0;
	stopped          = 1;
	pcb.next_burst   = 2+random()%(pcb.max_burst-1);

	reset_alrm();

	if ( pcb.ttl < pcb.next_burst ) pcb.next_burst = pcb.ttl;

	if ( pcb.last_duration >= pcb.ttl ) {
	  pcb.ttl  = 0;
	  raise( SIGINT );
	}
	else {
	  pcb.ttl -= pcb.last_duration;

	  fprintf( stderr, "\n"COLORED(BOLD,BLUE,"Stopped after %d ms")" (next_burst: "COLORED(BOLD,BLUE,"%d")" ms, TTL: "COLORED(HIGH,YELLOW,"%lu")" ms).",
		   pcb.last_duration, pcb.next_burst, pcb.ttl );

	  notify_sched( PROC_BURST, pcb.next_burst, pcb.ttl, pcb.last_duration );
	  raise( SIGSTOP );
	}
      }
      break;
    case STATE_INTERRUPT :
      if ( 0 == pcb.ttl ) {
	fprintf( stderr, "\n"COLORED(HIGH,YELLOW,"Finished work!")"\n" );
      }
      else {
	fprintf( stderr, "Received SIGINT\n" );
      }
      notify_sched( PROC_DELETE, 0, pcb.ttl, pcb.last_duration );
      pcb.state = STATE_DELETE;
      break;
    case STATE_DELETE :
      /* In case a SIGSTOP would have been sent during next getchar(3).. */
      LOAD_HANDLER( SIGUSR1, SIG_IGN );
      LOAD_HANDLER( SIGCONT, SIG_IGN ); /* Very much less an issue.      */
      break;
    }
  }

  fprintf( stderr, "Received SIGTERM\n" );
#ifdef DEBUG
  fprintf( stderr, "Press enter to continue..." );
  getchar();
#endif
  exit( EXIT_SUCCESS );
}
