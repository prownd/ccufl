/**
 * @file hashtable.h
 * @author dufei@nismail.iie.ac.cn
 * @version 3.1.1284
 */

#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__



typedef struct _hashtable_ {
    unsigned int tablelen;
    void **table;
    unsigned int offset;
	
    void (*_free)(void *);
    unsigned int (*_hash) (void *);
    int (*_cmp) (void *, void *);
	
	unsigned int nodecount;
    unsigned int loadlimit;
    unsigned int primeindex;
}hashtable_t;

/***************************************exported functions*****************************************/

/*
 * Create a array from heap space and return the pointer.
 * tablelen:	minimum initial length of hashtable
 * nodesize:	size of struct in hashtable
 * h_free:		function for free struct
 * h_hash:		function for hashing
 * h_cmp:		function for determining struct equality:if equality return 0 else return ~0
 */
hashtable_t * hashtable_create(unsigned int tablelen,unsigned int nodesize,void (*h_free)(void *),
             				unsigned int (*h_hash) (void*),int (*h_cmp) (void*,void*));
                 

void hashtable_init(hashtable_t *h); 

unsigned int hashtable_count(hashtable_t *h);                

/*
 * return:	0  succeed
 *		   -1  h is null
 */
int hashtable_insert(hashtable_t *h,void *it);

/*
 * return:	0  succeed
 *		   -1  h is null
 *		    1  MAX prime_number
 *		    2  alloc failed 
 */
int hashtable_insert_ex(hashtable_t *h,void *it);

/*
 * return:	0  succeed
 *			1  deletion failed
 *		   -1  h is null
 */
int hashtable_remove(hashtable_t *h, void *it);

void * hashtable_search(hashtable_t *h, void *it);

void hashtable_free(hashtable_t *h);

/*
 * traverse a array and handle each element.
 * h:			the pointer of hashtable
 * m:			the pointer of first param in callback
 * _callback:	function for computing
 */
void hashtable_traverse(hashtable_t *h,void *m,void (*_callback)(void *,void *));

void hashtable_show(hashtable_t *h,void (* _print)(void *));

#endif /* __HASHTABLE_H__ */


