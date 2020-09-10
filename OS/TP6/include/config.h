
/**
 * @file config.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Dec 29 09:56:31 2017
 * @brief Daemon config and constants.
 *
 * Daemon config and constants.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

  /* Give the user a chance to visually inspect 
     the life of a terminated process ,  or the 
     scheduling history. 
  */
#define DEBUG

  /* File containing the Scheduler PID.
   */
#define SCHED_PID "scheduler.pid"

  /* Time conversion. 
   */
#define SET_TIMER_IN_FAKE_MS(timer, val)				\
  (timer).it_value.tv_sec  = ((int)(DURATION_SCALE*1000*(val)))/(int)1e6; \
  (timer).it_value.tv_usec = ((int)(DURATION_SCALE*1000*(val)))%(int)1e6; \
  setitimer( ITIMER_REAL, &(timer), NULL )

#define RESET_TIMER_IN_FAKE_MS(timer) SET_TIMER_IN_FAKE_MS(timer, 0) 

#define TIME_DIFF_IN_FAKE_MS( after, before ) ((size_t)(((((after).tv_sec-(before).tv_sec)*1e6+((after).tv_usec-(before).tv_usec))/(DURATION_SCALE*1000))))

  /* Syntactic sugar for clarity. 
   */  
#define UNUSED(thing) if(thing) {}

#define CHECK_CALL( err_code, expr, msg ) if ( (err_code) == (expr) ) perror( msg )
  
#define LOAD_HANDLER(signum,handler) do { CHECK_CALL( SIG_ERR, signal( signum, handler ), "signal(2)" ); } while( 0 )

#define RELOAD_HANDLER(handler) LOAD_HANDLER(signum, handler)

  /* Pretty fprint(3)'ing.
   */

#define BLACK  "0m"
#define RED    "1m"
#define GREEN  "2m"
#define YELLOW "3m"
#define BLUE   "4m"
#define PURPLE "5m"
#define CYAN   "6m"
#define WHITE  "7m"

#define REGULAR     "\x1b[0;3"
#define BOLD        "\x1b[1;3"
#define UNDERLINE   "\x1b[4;9"
#define HIGH        "\x1b[0;9"
#define BOLD_HIGH   "\x1b[1;9"
#define COLOR_RESET "\x1b[0m"
  
#define COLORED(style, val, str) style val str COLOR_RESET
  
#ifdef __cplusplus
}
#endif

#endif /* _CONFIG_H_ */
