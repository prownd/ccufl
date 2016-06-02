#ifndef _PQUEUE_H_
#define _PQUEUE_H_

#include <stdint.h>
#include <pthread.h>

typedef struct pqueue_s pqueue_t;

struct pqueue_s
{
	unsigned int top;    //the top is always pointed to the newest one
	unsigned int tail;   //the tail is always pointed to the oldest one 
	unsigned int maxlen; //the fixed max length of the queue
	unsigned int len;    //the current length of the queue
	unsigned int nodesize; 
	pthread_spinlock_t lock;

	char *pq;			 //the head pointer to the queue
};

/*****************************************************************************/
/*
 *
 *
 * NULL: fail
 * NOT NULL: success, pointer to a queue
 */
pqueue_t *pqueue_create(unsigned int maxlen);

/*****************************************************************************/
void pqueue_init(pqueue_t *pq);

/*****************************************************************************/
void pqueue_free(pqueue_t *pq);

/*****************************************************************************/

int pqueue_count(pqueue_t *pq);

/**
 * no any limit to add a element into the queue.
 * 0: fail, is full
 * 1: success
 */
int pqueue_addone(pqueue_t *pq, void *node);

/*****************************************************************************/
/**
 * NULL: fail, error or null
 * NOT NULL: success
 */
void *pqueue_getone(pqueue_t *pq);

#endif /*_PQUEUE_H_*/
