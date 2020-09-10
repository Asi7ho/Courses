
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


  /*
    Constants. 
   */
#define _XOPEN_SOURCE 700
#define STRLEN 255

  /*
    Configuration values. 
   */
#define DAEMON       "cron"
#define LOCK_DAEMON  DAEMON".lock"
#define LOG_DAEMON   DAEMON".log"
#define CONF_DAEMON  DAEMON".conf"

  extern FILE *logfile;

#define UNUSED(thing) if(thing) {}

  void delete_lock_file( void );
  
#ifdef __cplusplus
}
#endif

#endif /* _CONFIG_H_ */
