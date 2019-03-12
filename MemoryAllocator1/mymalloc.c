#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "malloc.h"

static const size_t MB = 1024*1024; 

static block_header *FLOOR = NULL; 
                                   
static block_header *limit = NULL; 
static block_header *last = NULL;  

void *mymalloc (size_t size_asked) {

  if (size_asked == 0) return NULL; 

  size_asked = size4(size_asked); 
                                  

  if (FLOOR == NULL) { 
    FLOOR = (block_header *) sbrk(0);
    void * err = sbrk(MB+BH_SIZE);
    if (err == (void *) -1) return NULL;
    FLOOR->alloc = 0;
    FLOOR->size = MB;
    limit = (block_header *) FLOOR + MB + BH_SIZE;
    last = FLOOR; 
  }


  block_header *ptr = FLOOR; 
  size_t size_disp = 0; 
  int best_fit = 0; 
  int fit = 0; 
  int best_size = 0; 
  int limit_reached = 0; 
  block_header *best_ptr = NULL; 

  while (!best_fit && !limit_reached) {
    size_disp = 0; 

    while ((ptr + size_disp) < last && (ptr+size_disp)->alloc == 0 && size_disp != size_asked){
      size_disp += (ptr + size_disp)->size + BH_SIZE; 
    }

    if (ptr == last  || ptr + size_disp == last ) {
      limit_reached = 1; 
    }

    if (size_disp == size_asked){ 
      best_fit = 1;
      best_size = size_disp;
      best_ptr = ptr;
    }

    if (size_disp > size_asked) { 
      fit = 1;
      if (size_disp > best_size) { 
        best_ptr = ptr;
        best_size = size_disp;
      }
      ptr += ptr->size; 
    }

    if (size_disp < size_asked && !limit_reached) { 
      ptr += size_disp + BH_SIZE + (ptr+size_disp)->size; 
                                               
    }



  }

  if (!fit) {
    if (ptr+size_asked > limit) return NULL; 
    int mem = last->size; 
    ptr->alloc = 1; 
    ptr->size = size_asked; 
    last = ptr + BH_SIZE + size_asked; 
    last->alloc = 0; 
    last->size = mem - (size_asked - size_disp); 
    return ptr+BH_SIZE; 
  }

  if (best_size != size_asked) { 
	  (best_ptr+BH_SIZE+size_asked)->alloc = 0; 
    (best_ptr+BH_SIZE+size_asked)->size = best_size - size_asked - BH_SIZE; 
  }

  best_ptr->alloc = 1;
  best_ptr->size = size_asked;
  return best_ptr+BH_SIZE;

}

void *mycalloc (size_t size){
	void *ptr = mymalloc(size); 
  int *cl_ptr = (int *) ptr; 
	size = size4(size); 
	if (ptr == NULL) return NULL;
	int i;
	for (i =0; i < size; i++) *(cl_ptr+i) = 0; 
	return ptr; 
}

void myfree(void *ptr) {
  if (ptr==NULL) return; 
  block_header *bh_ptr = ptr-BH_SIZE; 
  if (bh_ptr >= limit) return; 
  if (bh_ptr < FLOOR) return; 
  bh_ptr->alloc = 0; 
}

