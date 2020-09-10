
/**
 * @file palloc.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 23 12:11:29 2018
 * @brief Protected alloc.
 *
 * Protected Alloc.
 */

#include <stdio.h>    /* fprintf */
#include <stdlib.h>   /* size_t */
#include <sys/mman.h> /* mmap, munmap, mprotect */
#include <string.h>   /* memset, strerror */
#include <errno.h>    /* errno */
#include <unistd.h>   /* sysconf */
#include <signal.h>


#define PALLOC_CANARY    0xDEADBEEF


void *pmalloc( size_t size ) {

  size_t    canary = PALLOC_CANARY;
  size_t page_size = sysconf( _SC_PAGESIZE );
  size_t    npages = 0;
  size_t     total = 0;//total number of BYTES needed
  char        *ptr = NULL;

  /*ok TODO: Compute 'total' number of bytes actually needed. */
  total=size+2*sizeof(size_t)+page_size;//space for canary, number of pages and dead page
  /*ok TODO: Init ptr with mmap(2), asking for 'total' bytes.
     Hint: flags = MAP_PRIVATE | MAP_ANONYMOUS).
  */
  ptr=mmap(NULL, total, (PROT_READ|PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS),0, 0);//fd don't care
/*  void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);*/

  /*
     In mmap(2), 'total' will be internally rounded up to
     a multiple of 'page_size' (the kernel only allocates
     *whole* pages).
  */

  /* We also need the number of pages that were allocated. */
  while( npages*page_size < total ) npages++;

  /*ok TODO: Call mprotect(2) to make a dead page at the end. */
  mprotect(ptr+page_size*(npages-1), page_size, PROT_NONE);
  /*ok TODO: Compute value of 'ptr' returned to caller. */
  ptr=ptr+page_size*(npages-1)-size;//on veut que la derniere adresse de l'utilisateur soit juste avant la page morte
  /*ok TODO: Place 'canary' and 'npages' where appropriate.
     Hint: Use memcpy(3).
   */
   memcpy( ptr-2*sizeof(size_t),&npages, sizeof(size_t));//une egalite ne fonctionnera pas a cause des types
   memcpy( ptr-1*sizeof(size_t),&canary, sizeof(size_t));

  return ptr;
}


void *pcalloc( size_t nmemb, size_t size ) {
  size_t total = nmemb*size;
  void    *ptr = pmalloc( total );

  /* pcalloc as a wrapper around pmalloc + memset */
  memset( ptr, 0, total );

  return ptr;
}


void  pfree( void *ptr ) {
  char       *rptr = ptr; /* No pointer arithmetic on void* */
  size_t page_size = sysconf( _SC_PAGESIZE );
  size_t    canary = 0;
  size_t    npages = 0;

  /*ok TODO: Read 'canary' and 'npages' back.
     Hint: Use memcpy(3).
   */
   memcpy(&npages, rptr-2*sizeof(size_t), sizeof(size_t));
   memcpy(&canary, rptr-1*sizeof(size_t), sizeof(size_t));
  /*ok TODO: Check canary, possibly emit warning. */
  if(canary!=PALLOC_CANARY) printf("Canary Warning! %zx %x\n",canary,PALLOC_CANARY);
  /*ok TODO: Call munmap(2) to actually free the mmap'd block. */
  munmap(rptr-2*sizeof(size_t),npages*page_size);
  return;
}
