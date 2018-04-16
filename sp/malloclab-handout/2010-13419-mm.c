/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your information in the following struct.
 ********************************************************/
student_t student = {
  /* Your full name */
  "Yongjae Lee",
  /* Your student ID */
  "2010-13419",
};

/* DON'T MODIFY THIS VALUE AND LEAVE IT AS IT WAS */
static range_t **gl_ranges;
static char *heap_listp;

static void *extend_heap(size_t);
static void *coalesce(void*);
static void *find_fit(size_t);
static void place(void*, size_t);


/* Basic constants and macros */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a szie and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated field from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous block */ 
#define NEXT_BP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* 
* remove_range - manipulate range lists
* DON'T MODIFY THIS FUNCTION AND LEAVE IT AS IT WAS
*/
static void remove_range(range_t **ranges, char *lo)
{
	range_t *p; 
	range_t **prevpp = ranges;
		 
	if (!ranges)
		return;

	for (p = *ranges;  p != NULL; p = p->next) {
		if (p->lo == lo) {
			*prevpp = p->next;
			free(p);
			break;
		}   
	prevpp = &(p->next);
	}
}

/*
* handle_double_free - prints an error message and aborts
*    when your program tries to free an unallocated or freed
*    memory block.
* DON'T MODIFY THIS FUNCTION AND LEAVE IT AS IT WAS
*/
void handle_double_free(void) {
	printf("ERROR : Your program tried to unallocated or freed \
	memory block.\n");
	exit(-1);
}


/* TODO
 * mm_init - initialize the malloc package.
 */
int mm_init(range_t **ranges)
{
  /* YOUR IMPLEMENTATION */
  if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) return -1;

	PUT(heap_listp, 0);
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (3*WSIZE), PACK(0, 1));
	heap_listp += (2*WSIZE);

	if (extend_heap(CHUNKSIZE/WSIZE) == NULL) return -1;

  /* DON't MODIFY THIS STAGE AND LEAVE IT AS IT WAS */
  gl_ranges = ranges;

  return 0;
}

static void *extend_heap(size_t words){
  char *bp;
	size_t size = ALIGN(words*WSIZE);

	if((long)(bp = mem_sbrk(size))==-1) return NULL;

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BP(bp)), PACK(0, 1));

	return coalesce(bp);
}

/* TODO
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void* mm_malloc(size_t size)
{
  int newsize;
//  void *p = mem_sbrk(newsize);
  char *bp;

  if(size == 0) return NULL;

  if(size <= DSIZE) newsize = 2*DSIZE;
	else newsize = ALIGN(size + 2*DSIZE);

	if((bp = find_fit(newsize))!=NULL) {
	  place(bp, newsize);
  } else {
		if((bp=extend_heap(MAX(newsize,CHUNKSIZE)/WSIZE))==NULL) return NULL;
    place(bp, newsize);
	}

  return bp;
/* LEGACY of old code by TA
  if (p == (void *)-1)
    return NULL;
  else {
    *(size_t *)p = size;
    return (void *)((char *)p + SIZE_T_SIZE);
  }
*/
}

static void *find_fit(size_t size){
  // First fit 
	void *tmpPtr = NEXT_BP(heap_listp);

  //it will stop at the epilogue block cause its size is 0.
	while(GET_SIZE(HDRP(tmpPtr))>0) {
	  if(!GET_ALLOC(HDRP(tmpPtr)) && size<=GET_SIZE(HDRP(tmpPtr))) return tmpPtr; 
		tmpPtr=NEXT_BP(tmpPtr);
	}
	return NULL;
}

static void place(void *bp, size_t asize){
  int oldsize = GET_SIZE(FTRP(bp));
	int surplus = oldsize-asize;

  //Split if surplus size is greather than minimum block size(2*DSIZE)
	if(surplus > DSIZE<<1) {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));

	  PUT(HDRP(NEXT_BP(bp)), PACK(surplus, 0));
		PUT(FTRP(NEXT_BP(bp)), PACK(surplus, 0));
	} else {
	  PUT(HDRP(bp), PACK(oldsize, 1));
		PUT(FTRP(bp), PACK(oldsize, 1));
	}
	
	return;
}

/* 
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
  /* YOUR IMPLEMENTATION */
  size_t size = GET_SIZE(HDRP(bp));
  
	if(GET_ALLOC(HDRP(bp))){
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
		
		coalesce(bp);
	} else {
    fprintf(stderr, "ERROR: It tried double free...\n");
		exit(-1);
	}

  /* DON't MODIFY THIS STAGE AND LEAVE IT AS IT WAS */
  if (gl_ranges)
    remove_range(gl_ranges, bp);
}

static void *coalesce(void *bp){
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BP(bp))); 
	size_t size = GET_SIZE(HDRP(bp));

  /* Case 1: both prev & next are allocated */
	if(prev_alloc && next_alloc) { 
    return bp;
	}

  /* Case 2: prev is allocated, next is free */
	else if(prev_alloc && !next_alloc){
    size += GET_SIZE(HDRP(NEXT_BP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}

	/* Case 3: prev is free, next is allocated */
	else if(!prev_alloc && next_alloc){
    size += GET_SIZE(FTRP(PREV_BP(bp)));
		PUT(HDRP(PREV_BP(bp)), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
		bp = PREV_BP(bp);
	}

	/* Case 4: both prev & next are free */
	else {
    size += GET_SIZE(HDRP(NEXT_BP(bp))) + GET_SIZE(FTRP(PREV_BP(bp)));
    PUT(HDRP(PREV_BP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BP(bp)), PACK(size, 0));
		bp = PREV_BP(bp);
	}
	return bp;
}

/* TODO
 * mm_realloc - empty implementation; YOU DO NOT NEED TO IMPLEMENT THIS
 */
void* mm_realloc(void *ptr, size_t t)
{
  return NULL;
}

/* TODO
 * mm_exit - finalize the malloc package.
 */
void mm_exit(void){
  char *tmpPtr = NEXT_BP(heap_listp);
  size_t tmpSize = GET_SIZE(HDRP(tmpPtr)); 
   
  while(tmpSize>0){
		if(GET_ALLOC(HDRP(tmpPtr))) {
			mm_free(tmpPtr);
		}
		tmpPtr = NEXT_BP(tmpPtr);
		tmpSize = GET_SIZE(HDRP(tmpPtr));
  }
	return;
}

int mm_check(void){
  return 1;
}
