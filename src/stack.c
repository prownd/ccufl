/**
 * @file stack.c
 * @time 2015-7-2
 * @author hanjinpeng127@163.com
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack.h"


struct _stack_{
	void *head;
	unsigned int nodecount;
	unsigned int offset;
	pthread_spinlock_t lock;
	void (*_free)(void *);
};

/***************************************************************************************************/
stack_t *stack_create(unsigned int nodesize,void (*s_free)(void *))
{
	unsigned int ptrlen = sizeof(void *);
	if(nodesize<=ptrlen){
		fprintf(stderr,"Err:nodesize is too small!\n");
		return NULL;
	}
	stack_t *stack=calloc(1,sizeof(stack_t));
	if(!stack){
		fprintf(stderr,"Err:memory fails to malloc!\n");
		return NULL;
	}
	
	stack->head=NULL;
	stack->nodecount=0;
	stack->offset=nodesize - ptrlen;
	stack->_free=s_free?s_free:free;
	pthread_spin_init(&stack->lock,PTHREAD_PROCESS_PRIVATE);
	return (stack);
}

/***************************************************************************************************/
void stack_free(stack_t *stack)
{
	if(NULL==stack) return;
	
	void *v,*rover=stack->head;
	
	while(NULL!=rover)
	{
		v = (void *)*(unsigned long *)((char*)rover+stack->offset);
		
		stack->_free(rover);
		
		rover=v;		
	}
	free(stack);
}

/***************************************************************************************************/
void stack_init(stack_t *stack)
{
	if(NULL==stack) return;

	/* Iterate over each node, freeing each stack node, until the end is reached */
	void *v,*rover=stack->head;
	
	while(NULL!=rover)
	{
		v = (void *)*(unsigned long *)((char*)rover+stack->offset);
		
		stack->_free(rover);
		
		rover=v;		
	}
	
	stack->nodecount=0;
	stack->head=NULL;
}

/***************************************************************************************************/
int stack_count(stack_t *stack)
{
	pthread_spin_lock(&stack->lock);
	if(NULL==stack) {
		pthread_spin_unlock(&stack->lock);
		return -1;
	}
	pthread_spin_unlock(&stack->lock);
	return (stack->nodecount);
}


/***************************************************************************************************/
void *stack_top(stack_t *stack)
{
	pthread_spin_lock(&stack->lock);
	if(!stack||!stack->head) {
			pthread_spin_unlock(&stack->lock);
			return NULL;
	}
	pthread_spin_unlock(&stack->lock);
	return (stack->head);
}

/***************************************************************************************************/
int stack_push(stack_t *stack, void *node)
{	
	pthread_spin_lock(&stack->lock);
	if(NULL==stack || NULL==node) {
		pthread_spin_unlock(&stack->lock);
		return 0;
	}

	if(stack->head==NULL){
		stack->head  = node;
	}else{
		/* Add to the head of stack */
		*(unsigned long *)((char*)node+stack->offset) = (unsigned long)stack->head;
		stack->head = node;
	}
	stack->nodecount++;
	pthread_spin_unlock(&stack->lock);
	return 1;
}


void *stack_pop(stack_t *stack)
{
	/* If the stack is empty, or data is NULL, always fail */
	pthread_spin_lock(&stack->lock);
	if (NULL==stack) {
			pthread_spin_unlock(&stack->lock);
			return 0;
	}
	
	void *rover;
	
	rover=stack->head;
	if(stack->nodecount==1)
		stack->head=NULL;	/* Just one node.*/			
	/* the current node is head.*/
	stack->head=(void *)*(unsigned long*)((char*)rover+stack->offset);	
		
	pthread_spin_unlock(&stack->lock);
	stack->nodecount--;
	return rover;
}



