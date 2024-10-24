/****************************************************************
 * This file is part of the ccufl project.                      *
 * @license:    GPLv3                                           *
 * @file:       slist.h                                         *
 * @version:    1.0                                             *
 * @author:     Han Jinpeng <hanjinpeng127@gmail.com>           *
 * @date:       2015-7-2                                        *
 * @website:    https://github.com/prownd/ccufl                 *
 ****************************************************************/

#ifndef __SLIST_H__
#define __SLIST_H__

typedef struct _slist_ slist_t;

/***************************************exported functions******************************************/

/*
 * Create a list from heap space and return the pointer.
 * [NOTE] the user_node need to have the 'next pointer' in last domain of node-struct.
 * l_free:	the callback to free the user's struct.
 * l_cmp:	~0 when arg1!=arg2;=0 when arg1==arg2
 */
extern slist_t *slist_create(unsigned int nodesize,void (*l_free)(void *),int (*l_cmp)(void *, void *));

extern void slist_free(slist_t *l);

/*
 * initial the list itself variable to 0s,and remove all the nodes in slist.
 */
extern void slist_init(slist_t *l);

/*
 * return the list's nodecount or -1 if the list is not exist.
 */
extern int slist_count(slist_t *l);

/*
 * Get the slist tail node_ptr.
 */
extern void *slist_get_tail_ptr(slist_t *l);

/*
 * Get the slist head node_ptr.
 */
extern void *slist_get_head_ptr(slist_t *l);

/*
 * Append node to the end of a slist.
 * succeed 1 or failed 0.
 */
extern int slist_append(slist_t *l, void *node);

/*
 * Prepend node to the start of a slist.
 * succeed 1 or failed 0.
 */
extern int slist_prepend(slist_t *l, void *node);

/*
 * Remove a particular node from a slist according to data.
 * succeed 1 or failed 0.
 */
extern int slist_remove(slist_t *l, void *data);

/*
 * Find the node for a particular data item in a slist.
 * return pointer of node or NULL if not found.
 */
extern void *slist_search(slist_t *l,void *data);

/*
 * Traversal the slist's user_data according to _func.
 * [from,to]: the same usage of array, eg [0,3]
 */
extern void slist_traverse(slist_t *l,void (*_func)(void*),unsigned int from,unsigned int to);

extern void slist_getone(slist_t *l,void **c,unsigned int n);

/*
 * Sort a slist.The sequence is from small to large.
 * sort_cmp: 	>0 when arg1>arg2; <0 when arg1<arg2; =0 when arg1==arg2				
 */
extern void slist_sort(slist_t *l, int (*sort_cmp)(void *, void *));

#endif /* __SLIST_H__ */

