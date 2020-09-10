
/**
 * @file daemon.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Dec 28 12:35:34 2017
 * @brief Daemon entry point.
 *
 * Daemon entry point.
 */

#include "config.h"
#include "logging.h"
#include "list.h"
#include "event.h"

#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Globals, because sometimes everyone feels lazy. */
list   events = nil;
FILE *logfile = NULL;


/*
   Lock file management.
*/


/* Returns 0 in case lock file does not exist or PID can't be read. */
static int lock_file_exists( void ) {
  FILE *fp = fopen( LOCK_DAEMON, "rt" );

  if ( !fp ) {
    return 0;
  }
  else {
    int pid = 0;

    if ( 1 != fscanf( fp, "%d", &pid ) ) {
      ABORT( "Can't read PID.\n" );
    }
    fclose( fp );

    return pid;
  }
}

/* Create lock file, store PID inside. */
static int create_lock_file( void ) {
  FILE *fp = fopen( LOCK_DAEMON, "w" );

  if ( !fp ) {
    ABORT( LOCK_DAEMON );
  }

  fprintf( fp, "%d\n", getpid() );

  fclose( fp );

  return 0;
}


/*
   Upon call to exit...
*/

void delete_lock_file( void ) {
  LOG( logfile, "* Deleting lock file.\n" );
  unlink( LOCK_DAEMON );
  return;
}

static void delete_event_list( void ) {
  LOG( logfile, "* Deleting event list.\n" );
  delete( events, del_event );
}

static void  close_log_file( void ) {
  LOG( logfile, "* Closing log file.\n" );
  fclose( logfile );
}


/* Find next alarm. */
static void set_next_alarm( void ) {

  if ( empty( events ) ) {
    LOG( logfile, "No next event.\n" );
  }
  else {
    event   ev = car( events );
    int   when = event_date( ev ) - time( NULL );

    LOG( logfile, "Next event in %d sec.\n", when );
    alarm( when );
  }
}


/*
   Signal handlers.
 */


static void sigalrm_handler( int signum ) {

  /*OK TODO: Reload signal handler?*/
  signal(SIGALRM, sigalrm_handler);//rearmement pour la  prochaine commande a executer

  if ( !empty( events ) ) {
    event   ev;
    char   buff[1+STRLEN];

    ev = pop( events );
    snprintf( buff, STRLEN, "Executing: %s.\n", event_command( ev ) );
    LOG( logfile, buff );
    set_next_alarm();
    system( event_command( ev ) );
  }

  return;
  UNUSED( signum );
}

static void sigint_handler( int signum ) {

  /*OK TODO: Reload signal handler? */
  signal(SIGINT, sigint_handler);//rearmement, on peut envoyer une autre demande de rechargement
  delete( events, del_event );

  events = read_events( CONF_DAEMON );

  LOG( logfile, "Events loaded.\n" );

  set_next_alarm();

  return;
  UNUSED( signum );
}

static void sigusr1_handler( int signum ) {
  int     n = length( events );

  /*OK TODO: Reload signal handler? */
  signal(SIGUSR1, sigusr1_handler);//rearmement, on peut envoyer une autre demande de lister les elements restants
  printf( "\nRunning with PID %d.\n", getpid() );

  if ( 0 == n ) {
    printf( "* No next event (reload updated config file).\n" );
  }
  else {
    printf( "* %d event%s remaining:\n\n", n, n > 1 ? "s" : "" );
    visit( events, print_event );
  }

  UNUSED( signum );
}

static void sigterm_handler( int signum ) {

  /*OK TODO: Reload signal handler? */
  /*no need to reload*///pas de rearmement, on quitte le programme

  LOG( logfile, "Exiting.\n" );

  exit( EXIT_SUCCESS );

  UNUSED( signum );
}

/*
  Daemon entry point.
 */
static void main_daemon( int argc, char *argv[] ) {

  /*OK TODO: Register functions to be called on exit(3) !in reverse order! */
  atexit(close_log_file);
  atexit(delete_lock_file);
  atexit(delete_event_list);

  /*OK TODO: Register signal handlers. */
  signal(SIGALRM, sigalrm_handler);//armement de tous les signaux
  signal(SIGTERM, sigterm_handler);
  signal(SIGUSR1, sigusr1_handler);
  signal(SIGINT, sigint_handler);
  /* Create log and lock files. */
  logfile = open_log_file( LOG_DAEMON );

  create_lock_file();

  /* Start logging. */
  LOG( logfile, "Up and running.\n" );

  /*OK TODO: Force loading of config file (send SIGINT to self). */
  kill(getpid(),SIGINT);
  /*OK TODO: Wait for signals. */
  while(1) pause();//sans pause ça marche aussi mais ça occupe le processeur a rien faire
  return;
  UNUSED( argc );
  UNUSED( argv );
}

/* Create daemon. */
static int daemonize( void (*exec_daemon)(int,char*[]), int argc, char *argv[] ) {

  /*OK TODO: Use double fork and setsid to launch daemon entry point. */
  if(fork()==0)//child
  {
    setsid();//deconnexion du clavier
    if(fork()==0)//child
    {
      exec_daemon(argc, argv);//lancement du demon
    }
    else
    {
      exit(EXIT_SUCCESS);//fin du pere
    }
  }
  else
  {
    exit(EXIT_SUCCESS);//fin du pere
  }
  return EXIT_SUCCESS;
}


/* Cosmetics. */
static void print_usage( FILE *fp, char *progname ) {

  fprintf( fp, "\n%s  - A toy cron daemon.\n\n", progname );
  fprintf( fp, "  Usage: %s [--]command\n\n", progname );
  fprintf( fp, "  Available commands:\n\n" );
  fprintf( fp, "    help\tPrint this help.\n" );
  fprintf( fp, "    start\tStart %s daemon.\n", progname );
  fprintf( fp, "    stop\tStop %s daemon.\n", progname );
  fprintf( fp, "    restart\tRestart %s daemon.\n", progname );
  fprintf( fp, "    reload\tReload %s config file.\n", progname );
  fprintf( fp, "    status\tPrint %s daemon status.\n", progname );
  fprintf( fp, "\n" );
}


/* Main entry point, check args */
int main ( int argc, char *argv[] ) {

  int        pid = -1;
  char *progname = argv[0];

  /* Remove garbage at start of argv[0] */
  while( !isalpha( *(progname++) ) ); progname--;

  if ( 2 != argc ) {
    print_usage( stderr, progname );
    exit( EXIT_FAILURE );
  }

  /* If non-zero, this is the PID of our daemon. */
  pid = lock_file_exists();

  /*OK TODO: Handle 'restart' command (first, because of strstr and 'start' command).
     Hint: Report failure on exiting if daemon is not running,
     otherwise terminate it and launch new one.
   */
  if ( strstr( argv[1], "restart" ) ) {
    if ( !pid ) {
      fprintf( stderr, "No %s daemon running.\n", progname );
      exit( EXIT_FAILURE );
    }
    else {
      kill(pid,SIGTERM);//on arrete le demon existant
      daemonize(main_daemon,argc,argv);//on en demarre un nouveau
      exit(EXIT_SUCCESS);
    }
  }

  /*OK TODO: Handle 'start' command.
     Hint: If daemon is already running, report failure on exiting.
     Otherwise, launch daemon entry point.
   */
  if ( strstr( argv[1], "start" ) ) {
    if ( pid ) {
      fprintf( stderr, "%s running, not launching daemon.\n", progname );
      exit( EXIT_FAILURE );
    }
    else {
      daemonize(main_daemon,argc,argv);
      exit(EXIT_SUCCESS);
    }
  }

  /*OK TODO: Handle 'stop' command.
     Hint: If daemon is not running, report failure on exiting.
     Otherwise, terminate it.
   */
  if ( strstr( argv[1], "stop" ) ) {
    if ( !pid ) {
      fprintf( stderr, "No %s daemon running.\n", progname );
      exit( EXIT_FAILURE );
    }
    else {
      kill(pid,SIGTERM);
      exit(EXIT_SUCCESS);
    }
  }

  /*OK TODO: Handle 'status' command.
     Hint: If daemon is not running, report failure on exiting.
     Otherwise, send SIGUSR1 signal.
   */
  if ( strstr( argv[1], "status" ) ) {
    if ( !pid ) {
      fprintf( stderr, "No %s daemon running.\n", progname );
      exit( EXIT_FAILURE );
    }
    else {
      kill(pid,SIGUSR1);
      exit(EXIT_SUCCESS);
    }
  }

  /*OK TODO: Handle 'reload' command.
     Hint: If daemon is not running, report failure on exiting.
     Otherwise, reload config file.
   */
  if ( strstr( argv[1], "reload" ) ) {
    if ( !pid ) {
      fprintf( stderr, "No %s daemon running.\n", progname );
      exit( EXIT_FAILURE );
    }
    else {
      kill(pid,SIGINT);
      exit(EXIT_SUCCESS);
    }
  }

  if ( strstr( argv[1], "help" ) ) {
    print_usage( stdout, progname );
    exit( EXIT_SUCCESS );
  }

  /* No known command, report failure on exiting. */
  print_usage( stderr, progname );
  exit( EXIT_FAILURE );
}
