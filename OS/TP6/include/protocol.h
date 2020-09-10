
/**
 * @file protocol.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Feb 10 16:54:44 2018
 * @brief Protocol for process queue.
 *
 * Protocol for process queue.
 */

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif


#define PROC_CREATE 1
#define PROC_DELETE 2
#define PROC_YIELD  3
#define PROC_BURST  4

  typedef struct { 
    long             type; 
    pid_t             pid; 
    size_t            ttl; 
    size_t  last_duration; 
    size_t     next_burst; 
  } proc_q_msg; 

#define PROC_Q_MSGSZ (sizeof(proc_q_msg)-sizeof(long))
  
#ifdef __cplusplus
}
#endif

#endif /* _PROTOCOL_H_ */
