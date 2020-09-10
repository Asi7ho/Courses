
/**
 * @file op_mmult_par.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Jan 31 19:19:26 2018
 * @brief Parallel matrix multiplication (fine grained).
 *
 * Parallel matrix multiplication (fine grained).
 */


#include <errno.h>
#include <pthread.h>


#include "api.h"

/*
  By default, no synchronization occurs.

  Use compare and swap sync with:
  #define SYNC_COMPARE_AND_SWAP

  Use pthread mutex sync with:
  #define SYNC_MUTEX

 */

/* TODO: Use compare and swap sync  to check for
   accuracy of the numerical result vs. no sync.
 */

 //#define SYNC_COMPARE_AND_SWAP
 #define SYNC_MUTEX

/* Ad-hoc thread arguments structure. */
typedef struct {
  size_t               r; /* row index            */
  size_t               c; /* column index         */
  size_t               k; /* 0 <= k < dim         */
  mat                 m1;
  mat                 m2;
  mat                res; /* Output matrix: m1*m2 */
#ifdef SYNC_MUTEX
  pthread_mutex_t  *lock;
#endif
#ifdef SYNC_COMPARE_AND_SWAP
  /* Tell the compiler to issue an  explicit
     memory reference at each access because
     the lock value may change at any time .
  */
  volatile int     *lock;
#endif
} thread_args;




#ifdef SYNC_MUTEX
#define USE_SYNC

/* TODO: Acquire the lock using the pthread mutex. */
static void  acquire_lock( pthread_mutex_t *lock ) {
  if (pthread_mutex_lock(lock) != 0){

  }
}

/* TODO: Release the lock using the pthread mutex. */
static void  release_lock( pthread_mutex_t *lock ) {
  if (pthread_mutex_unlock(lock) != 0){

  }
}
#endif



#ifdef SYNC_COMPARE_AND_SWAP
#define USE_SYNC

/*
  Atomic implementation of:

  int old_lock_val = *lock;
  if ( *old_lock_val == go ) *lock = keep_out;
  return old_lock_val;

  On multi-core Intel platforms , 'cmpxchg' must be
  prefixed with 'lock;' to make it actually atomic.

 */
static unsigned int compare_and_swap( volatile int *lock, int go, int keep_out ) {
  int           old_lock_val;

  /* Cheating on the Linux kernel source code: */
  asm volatile ( "lock; cmpxchg %2, %1"
                 :"=a" (old_lock_val), "+m" (*lock)
 		 :"r" (keep_out), "0" (go)
 		 :"memory" );

  return old_lock_val;
}

#define KEEP_OUT 1
#define GO       0

static void  acquire_lock( volatile int *lock ) {

  while ( KEEP_OUT == compare_and_swap( lock, GO, KEEP_OUT ) );

  /* Alternative: use sched_yield(2) instead of
     NOP . Puts more pressure on the scheduler.
  */
}

static void  release_lock( volatile int *lock ) {

  *lock = GO;
}
#endif



/* Threads main entry point. */
static void *thread_main( void *ptr ) {
  thread_args *args = ptr;

  /* Critical section starts. */
#ifdef USE_SYNC
  acquire_lock( args->lock );
#endif

  /* Threads actual mind-blowing job ;) */
  args->res[ args->r ][ args->c ] += args->m1[ args->r ][ args->k ] * args->m2[ args->k ][ args->c ];


  /* Critical section ends. */
#ifdef USE_SYNC
  release_lock( args->lock );
#endif

  return NULL;
}




/* Parallel computation of one output matrix coefficient. */
static void compute_coeff( mat m1, mat m2, mat res, size_t r, size_t c, size_t dim ) {
  size_t                       k = 0;
  pthread_t                 *tid = calloc( dim, sizeof( *tid ) );
  thread_args           *thr_arg = calloc( dim, sizeof( *thr_arg ) );
  size_t           up_to_success = 0;
#ifdef SYNC_MUTEX
  /* TODO: Declare pthread mutex. Use standard initializer. */
  pthread_mutex_t lock;
#endif
#ifdef SYNC_COMPARE_AND_SWAP
  int                       lock = GO;
#endif
  /* Prepare thread arguments array */
  for ( k = 0; k < dim; k++ ) {
    thr_arg[ k ].r    = r;
    thr_arg[ k ].c    = c;
    thr_arg[ k ].k    = k;
    thr_arg[ k ].m1   = m1;
    thr_arg[ k ].m2   = m2;
    thr_arg[ k ].res  = res;
#ifdef SYNC_MUTEX
    /* TODO: Make the mutex available to all threads. */
    thr_arg[k].lock = &lock;
#endif
#ifdef SYNC_COMPARE_AND_SWAP
    thr_arg[ k ].lock = &lock;
#endif
  }

  /* TODO: Launch the 'dim' threads.

     Rationale: We use standard threads 'attr' (NULL) .  Hence, only
     EAGAIN error (no more resources to create the thread) can occur
     when calling pthread_create(3).
     Therefore, on pthread_create(3) error , use pthread_join(3) to
     wait for the termination of the running threads . The variable
     'up_to_success'  is  used  to  monitor the number  of  threads
     already joined *before* the final call to pthread_join(3).
     Remotely resembles a barrier.
   */
for (k = 0; k < dim; k++){
  if (pthread_create(&tid[k], NULL, &thread_main, &thr_arg[k]) != 0) { //1 = identifiant du thread; 2 = NULL; 3 = point d'entree du thread; 4 = pointeur sur parametres du thread
    printf("Erreur de creation de threat\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(tid[k], (void**)&thr_arg[k]) != 0){
    printf("Erreur de threat_join\n");
    exit(EXIT_FAILURE);
  }
}

  /* TODO: Join (remaining) threads. */


  free( tid );
  free( thr_arg );

  return;
}


/* Plugin entry point */
object op_mmult_par( shell s, object op ) {
  object    args = op_args( op );
  object   first = car( args );
  object  second = cadr( args );
  mat         m1 = object_mat( first );
  mat         m2 = object_mat( second );
  mat        res = new_mat( mat_height( m1 ), mat_width( m2 ), gen_zeros );
  size_t       h = mat_height( res );
  size_t       w = mat_width( res );
  size_t     dim = mat_width( m1 );
  size_t    r, c;

  for ( r = 0; r < h; r++ ) {
    for ( c = 0; c < w; c++ ) {
      compute_coeff( m1, m2, res, r, c, dim );
    }
  }

  return make( T_MAT, res );

  UNUSED( s );
}
