#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pqueue.h"
 
/*****************************************************************************/
/*
 *
 *
 * NULL: fail
 * NOT NULL: success, pointer to a queue
 */
pqueue_t *pqueue_create(unsigned int maxlen)
{	
	pqueue_t* pq;
	
	if(!maxlen)
		return NULL;
	pq=(pqueue_t*)malloc(sizeof(pqueue_t)+sizeof(void *)*maxlen);
	if(pq==NULL)
		return pq;
	memset(pq, 0, sizeof(pqueue_t)+sizeof(void *)*maxlen);
	//others are inited as 0
	pq->maxlen=maxlen;
	pq->nodesize=sizeof(void *);
	pthread_spin_init(&pq->lock, PTHREAD_PROCESS_PRIVATE);

	pq->pq=(char*)pq+sizeof(pqueue_t);

    return pq;
}

/*****************************************************************************/
void pqueue_init(pqueue_t *pq)
{
    if(!pq)
        return;
    int i=0;
    void *node;
	while(i<pq->len)
	{
	    node = (void *)(*(unsigned long *)(pq->pq+(pq->top+i)*pq->nodesize));
	    if(node)
	        free(node);
	    i++;
	}
	memset(pq, 0, sizeof(pqueue_t)+sizeof(void *)*pq->maxlen);
} 

/*****************************************************************************/
void pqueue_free(pqueue_t *pq)
{
    if(!pq)
        return;
    int i=0;
    void *node;
	while(i<pq->len)
	{
	    node=(void *)(*(unsigned long *)(pq->pq+(pq->top+i)*pq->nodesize));
	    if(node)
	        free(node);
	    i++;
	}
	free(pq);
}

/**
 *
 *
 */
int pqueue_count(pqueue_t *pq)
{
	int count = 0;

	if(pq == NULL)
		return -1;

	count = pq->len;

	return count;
}

/*****************************************************************************/

/*
 * no any limit to add a element into the queue.
 * 0: fail, is full
 * 1: success
 */
int pqueue_addone(pqueue_t *pq, void *node)
{	
	pthread_spin_lock(&pq->lock);	
	if((pq->top+1)%pq->maxlen==pq->tail){
		pthread_spin_unlock(&pq->lock);
	    return 0;
	}

    *(unsigned long *)(pq->pq+pq->top*pq->nodesize)=(unsigned long)node;
    pq->top=(pq->top+1)%pq->maxlen;
    pq->len++;
	pthread_spin_unlock(&pq->lock);
 
    return 1;
}

/*****************************************************************************/
/*
 * 
 * NULL: fail, error or null
 * NOT NULL: success
 */
void *pqueue_getone(pqueue_t *pq)
{
	void *pone;

	pthread_spin_lock(&pq->lock);
	if(pq->top==pq->tail){
		pthread_spin_unlock(&pq->lock);
		return NULL;
	}
	
	pone=(void *)(*(unsigned long *)(pq->pq+pq->tail*pq->nodesize));
	pq->tail=(pq->tail+1)%pq->maxlen;
	pq->len--;
	pthread_spin_unlock(&pq->lock);
	return pone;
}

/*********************************************************************/

