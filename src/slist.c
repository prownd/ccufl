/**
 * @file slist.c
 * @time 2015-7-2
 * @author hanjinpeng127@163.com
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "slist.h"


struct _slist_{
	void *head;
	void *tail;
	unsigned int nodecount;
	unsigned int offset;
	pthread_spinlock_t lock;
	void (*_free)(void *);
	int (*_cmp)(void *, void *);
};

/***************************************************************************************************/
slist_t *slist_create(unsigned int nodesize,void (*l_free)(void *),int (*l_cmp)(void *, void *))
{
	unsigned int ptrlen = sizeof(void *);
	if(nodesize<=ptrlen){
		fprintf(stderr,"Err:nodesize is too small!\n");
		return NULL;
	}
	slist_t *l=calloc(1,sizeof(slist_t));
	if(!l){
		fprintf(stderr,"Err:memory fails to malloc!\n");
		return NULL;
	}
	
	l->head=l->tail=NULL;
	l->nodecount=0;
	l->offset=nodesize - ptrlen;
	l->_free=l_free?l_free:free;
	l->_cmp=l_cmp?l_cmp:NULL;
	pthread_spin_init(&l->lock,PTHREAD_PROCESS_PRIVATE);
	return (l);
}

/***************************************************************************************************/
void slist_free(slist_t *l)
{
	if(NULL==l) return;
	
	/* Iterate over each node, freeing each list node, until the end is reached */
	void *v,*rover=l->head;
	
	while(NULL!=rover)
	{
		v = (void *)*(unsigned long *)((char*)rover+l->offset);
		
		l->_free(rover);
		
		rover=v;		
	}
	free(l);
}

/***************************************************************************************************/
void slist_init(slist_t *l)
{
	if(NULL==l) return;

	/* Iterate over each node, freeing each list node, until the end is reached */
	void *v,*rover=l->head;
	
	while(NULL!=rover)
	{
		v = (void *)*(unsigned long *)((char*)rover+l->offset);
		
		l->_free(rover);
		
		rover=v;		
	}
	
	l->nodecount=0;
	l->head=l->tail=NULL;
}

/***************************************************************************************************/
int slist_count(slist_t *l)
{
	pthread_spin_lock(&l->lock);
	if(NULL==l) {
		pthread_spin_unlock(&l->lock);
		return -1;
	}
	pthread_spin_unlock(&l->lock);
	return (l->nodecount);
}

/***************************************************************************************************/
void *slist_get_tail_ptr(slist_t *l)
{
	pthread_spin_lock(&l->lock);
	if(!l||!l->tail) {
			pthread_spin_unlock(&l->lock);
			return NULL;
	}
	pthread_spin_unlock(&l->lock);
	return (l->tail);
}

/***************************************************************************************************/
void *slist_get_head_ptr(slist_t *l)
{
	pthread_spin_lock(&l->lock);
	if(!l||!l->head) {
			pthread_spin_unlock(&l->lock);
			return NULL;
	}
	pthread_spin_unlock(&l->lock);
	return (l->head);
}

/***************************************************************************************************/
int slist_prepend(slist_t *l, void *node)
{	
	pthread_spin_lock(&l->lock);
	if(NULL==l || NULL==node) {
		pthread_spin_unlock(&l->lock);
		return 0;
	}

	/* Hook into the slist start */
	if(l->head==NULL)
		l->head = l->tail = node;
	else{
		/* Add to the fist of slist */
		*(unsigned long *)((char*)node+l->offset) = (unsigned long)l->head;
		l->head = node;
	}
	l->nodecount++;
	pthread_spin_unlock(&l->lock);
	return 1;
}

/***************************************************************************************************/
int slist_append(slist_t *l, void *node)
{
	pthread_spin_lock(&l->lock);
	if(NULL==l || NULL==node) {
			pthread_spin_unlock(&l->lock);
			return 0;
	}
	
	/* Hooking into the slist end */
	if(NULL==l->head){
		l->head = l->tail = node;
	}
	else{
		/* Add to the end of slist */
		*(unsigned long *)((char*)l->tail+l->offset) = (unsigned long)node;
		l->tail = node;
	}
	l->nodecount++;
	pthread_spin_unlock(&l->lock);
	return 1;
}

/***************************************************************************************************/
int slist_remove(slist_t *l, void *data)
{
	/* If the list is empty, or data is NULL, always fail */
	pthread_spin_lock(&l->lock);
	if (NULL==l || NULL==data || NULL==l->_cmp) {
			pthread_spin_unlock(&l->lock);
			return 0;
	}
	
	void *tmp,*prev,*rover;	/* points of node in slist*/
	
	for(prev=rover=l->head; rover != NULL; prev=rover,rover=tmp)
	{
		tmp=(void *)*(unsigned long *)((char*)rover+l->offset);
		
		if(l->_cmp(rover, data) == 0)
		{
			if(l->nodecount==1)
				l->head=l->tail=NULL;	/* Just one node.*/			
			else if(tmp==NULL) {
				l->tail=prev;	/* the current node is tail.*/				
				*(unsigned long *)((char*)prev+l->offset)=(unsigned long)0;
			} 			
			else if(l->head==rover) /* the current node is head.*/
				l->head=(void *)*(unsigned long*)((char*)rover+l->offset);	
			else					/* the current node is middle.*/
				*(unsigned long *)((char*)prev+l->offset)=*(unsigned long*)((char*)rover+l->offset);	
			
			l->_free(rover);
			pthread_spin_unlock(&l->lock);
			l->nodecount--;
			return 1;
		}
		
	}
	/* Not found */
	pthread_spin_unlock(&l->lock);
	return 0;
}

/***************************************************************************************************/
void *slist_search(slist_t *l,void *data)
{
	/* If the list is empty, or data is NULL, always fail */
	pthread_spin_lock(&l->lock);
	if (!l || !data || !l->_cmp) {
			pthread_spin_unlock(&l->lock);
			return NULL;
	}
	void *v,*rover=l->head;
	/* Iterate over nodes in the slist until the data is found */
	while(NULL!=rover)
	{
		v = (void *)*(unsigned long *)((char*)rover+l->offset); 
		
		if(l->_cmp(rover, data) == 0) return rover;
		
		rover=v;
	}
	/* Not found */
	pthread_spin_unlock(&l->lock);
	return NULL;
}

/***************************************************************************************************/
void slist_traverse(slist_t *l,void (*_func)(void*),unsigned int from,unsigned int to)
{
	pthread_spin_lock(&l->lock);
	if(NULL==l || NULL==_func||to >= l->nodecount ||from > to) {
		pthread_spin_unlock(&l->lock);
		return;
	}
	void *rover;
	unsigned int index;
	for(rover=l->head,index=0;rover!=NULL;index++)
	{
		if(index >= from && index <= to) _func(rover);
		rover = (void *)*(unsigned long *)((char*)rover+l->offset);
	}
	pthread_spin_unlock(&l->lock);
}

void slist_getone(slist_t *l,void **c,unsigned int n)
{
	pthread_spin_lock(&l->lock);
	if(NULL==l||n >= l->nodecount-1) {
			pthread_spin_unlock(&l->lock);
			return;
	}
	void *rover;
	unsigned int index;
	for(rover=l->head,index=0;rover!=NULL;index++)
	{
		if(index == n){ *c=rover;return;}
		rover = (void *)*(unsigned long *)((char*)rover+l->offset);
	}
	pthread_spin_unlock(&l->lock);
}

/***************************************************************************************************/
static void * /* return the last node in the new sorted slist */
slist_sort_internal(slist_t *l,void **list, int (*sort_cmp)(void *, void *))
{
	void *pivot,*rover;
	void *less_list, *more_list;
	void *less_list_end, *more_list_end;
	
	/* If there are less than two nodes in this slist, it is already sorted */
	if (*list == NULL || *(unsigned long *)((char*)(*list)+l->offset) == 0) return *list;
	
	/* The first node is the pivot */
	pivot = *list;

	/* Iterate over the list, starting from the second node.  Sort all nodes into the less and more lists 
	 * based on comparisons with the pivot */
	less_list = NULL;
	more_list = NULL;
	rover = (void *)*(unsigned long *)((char*)(*list)+l->offset);

	while (NULL != rover)
	{
		void *v = (void *)*(unsigned long *)((char*)rover+l->offset);
		if (sort_cmp(rover, pivot) < 0) {
			/* Place this in the less list */
			*(unsigned long *)((char*)rover+l->offset)=(unsigned long)less_list;
			less_list = rover;
		}
		else{
			/* Place this in the more list */
			*(unsigned long *)((char*)rover+l->offset)=(unsigned long)more_list;
			more_list = rover;
		}
		rover = v;
	}
	/* Sort the sublists recursively */
	less_list_end = slist_sort_internal(l,&less_list, sort_cmp);
	more_list_end = slist_sort_internal(l,&more_list, sort_cmp);
	
	/* Create the new list starting from the less list */
	*list = less_list;
	/* Append the pivot to the end of the less list.  If the less list was empty, start from the pivot */
	if (less_list == NULL) *list = pivot;
	else *(unsigned long *)((char*)less_list_end+l->offset)=(unsigned long)pivot;

	/* Append the more list after the pivot */
	*(unsigned long *)((char*)pivot+l->offset)=(unsigned long)more_list;

	/* Work out what the last node in the list is.  If the more list was  empty, the pivot was the last node. 
	 * Otherwise, the end of the  more list is the end of the total list. */
	if(more_list == NULL) return pivot;
	else return more_list_end;
}

void slist_sort(slist_t *l, int (*sort_cmp)(void *, void *))
{
	pthread_spin_lock(&l->lock);
	slist_sort_internal(l,&(l->head), sort_cmp);
	pthread_spin_unlock(&l->lock);
}

/*end of __SLIST_C__*/



