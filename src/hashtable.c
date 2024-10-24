/****************************************************************
 * This file is part of the ccufl project.                      *
 * @license:    GPLv3                                           *
 * @file:       hashtable.c                                     *
 * @version:    1.0                                             *
 * @author:     Han Jinpeng <hanjinpeng127@gmail.com>           *
 * @date:       2015-7-2                                        *
 * @website:    https://github.com/prownd/ccufl                 *
 ****************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

/*
 Credit for primes table: Aaron Krowne
 http://br.endernet.org/~akrowne;http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
*/
static const unsigned int primes[] = {
	53, 97, 193, 389,769, 1543, 3079, 6151,12289, 24593, 49157, 98317,	196613, 393241, 786433, 1572869,
	3145739, 6291469, 12582917, 25165843,50331653, 100663319, 201326611, 402653189,805306457, 1610612741
};
const unsigned int prime_table_length = sizeof(primes)/sizeof(primes[0]);
const float max_load_factor = 0.618;

static void _hfree(void *it)
{
	free(it);
}

static unsigned int _hhash (void *it)
{
	return (unsigned long)it;
}

static int _hcmp(void *pairA,void *pairB)
{
	return (unsigned long)pairA-(unsigned long)pairB;
}
/**************************************************************************************************/
hashtable_t * hashtable_create(unsigned int tablelen,unsigned int nodesize,void (*h_free)(void *),
							   unsigned int (*h_hash) (void*),int (*h_cmp) (void*,void*))
{
	hashtable_t *h=NULL;
    unsigned int pindex, size;
    int ptr_len;	
    size = primes[0];
    ptr_len = sizeof(void *);	
	
	if(nodesize<=ptr_len){
		printf("[error]:Struct size is too small..\n");
		return NULL;
	}
	tablelen= tablelen/max_load_factor +1;
	/* Check requested hashtable isn't too large */
	if(tablelen > (1u << 30)) return NULL;
	/* Enforce size as prime */
    for(pindex=0; pindex < prime_table_length; pindex++) {
        if (primes[pindex] > tablelen) { size = primes[pindex]; break; }
    }
	h = (hashtable_t *)calloc(1,sizeof(hashtable_t));
    if (NULL == h) return NULL; 	

	h->table =(void **)calloc(size,sizeof(void *));
	if (NULL == h->table) return NULL; 

	h->tablelen = size;
    h->offset = nodesize - ptr_len;
    h->_free = h_free?h_free:_hfree;
    h->_hash = h_hash?h_hash:_hhash;
    h->_cmp = h_cmp?h_cmp:_hcmp;

    h->nodecount = 0;    
    h->primeindex = pindex;    
	h->loadlimit = size * max_load_factor+1;	

	return h;
}

/**************************************************************************************************/
void hashtable_init(hashtable_t *h)
{
	if(!h) return;

	int i;
	void *cur, *alter;
	
	for (i = 0; i < h->tablelen; i++)
	{
	    cur = h->table[i];
	    while (NULL != cur){ 
	        alter = cur; 
			cur = (void *)*(unsigned long *)(cur+h->offset);
			h->_free(alter); 
		}
	}
    memset(h->table, 0, h->tablelen * sizeof(void *));
	h->nodecount = 0;
}
/**************************************************************************************************/
unsigned int hashtable_count(hashtable_t *h)
{
	if(!h)return -1;
	
    return h->nodecount;
}

/**************************************************************************************************/
static int hashtable_expand(hashtable_t *h)
{
	if(!h)return -1;
	
	void *cur=NULL,**p=NULL, **newtable=NULL;
    unsigned int newsize, i, index;
    
    /* Double the size of the table to accomodate more entries */
    if (h->primeindex == (prime_table_length - 1)) return 1;
    newsize = primes[++(h->primeindex)];
    newtable = (void **)realloc(h->table, newsize * sizeof(void*));
	if (NULL == newtable) { (h->primeindex)--; return 2; }
	
	h->table = newtable;
	memset(&newtable[h->tablelen], 0, (newsize - h->tablelen)*sizeof(void *));
	for(i = 0; i < h->tablelen; i++){
		for(p = &(newtable[i]), cur = *p; cur != NULL; cur = *p){
			index = h->_hash(cur)%newsize;
			if(index == i){
				p = cur+h->offset;
			}
			else{
				*p = (void *)*(unsigned long *)(cur+h->offset);
				*(unsigned long *)(cur+h->offset) = (unsigned long)newtable[index];
				newtable[index] = cur;
			}
		}
	}
	
    h->tablelen = newsize;
    h->loadlimit   = newsize * max_load_factor+1;
	
    return 0;
}


/**************************************************************************************************/
int hashtable_insert(hashtable_t *h, void *it)
{
	if(!h)return -1;

	unsigned int index; 

	h->nodecount += 1;
	index = h->_hash(it)%h->tablelen;
	*(unsigned long *)(it+h->offset)  = (unsigned long)h->table[index];
    h->table[index] = it;
    
    return 0;
}

/**************************************************************************************************/
int hashtable_insert_ex(hashtable_t * h,void * it)
{
	if(!h)return -1;	
	unsigned int index;
	int ret;
	
	/* This method allows expanding*/
	if(++(h->nodecount) > h->loadlimit)
	{		
		if(0!=(ret=hashtable_expand(h))) return ret;
	}
	index = h->_hash(it)%h->tablelen;
	*(unsigned long *)(it+h->offset)  = (unsigned long)h->table[index];
    h->table[index] = it;
	
    return 0;
}

/**************************************************************************************************/
int hashtable_remove(hashtable_t *h, void *it)
{
	if(!h)return -1;
	
	void *cur=NULL, **pE=NULL; 	
    unsigned int index;
	
    index=h->_hash(it)%h->tablelen;	
    pE = &(h->table[index]);
    cur = *pE;
    while (NULL != cur)
    {
        if (0==h->_cmp(it,cur)){
            *pE = (void *)*(unsigned long *)(cur+h->offset);
            h->nodecount--;
            h->_free(cur);
			cur=NULL;//add for array
            return 0;
        }
        pE = cur+h->offset;
		cur = (void *)*(unsigned long *)(cur+h->offset);
    }
    return 1;
}

/**************************************************************************************************/
void * hashtable_search(hashtable_t *h, void *it)
{
	if(!h)return NULL;
	
	void *cur=NULL;
    unsigned int index;

	index = h->_hash(it)%h->tablelen;
	cur = h->table[index];
    while (NULL != cur){
		if (0==h->_cmp(it, cur)) return cur;
        cur = (void *)*(unsigned long *)(cur+h->offset);
	}
	return NULL;
}

/**************************************************************************************************/
void hashtable_free(hashtable_t *h)
{
	if(!h)return; 

    unsigned int i;
    void *cur=NULL, *alter=NULL;
	
	for (i = 0; i < h->tablelen && h->nodecount>0; i++)
    {
        cur = h->table[i];
        while (NULL != cur){ 
        	alter = cur; 
			cur = (void *)*(unsigned long *)(cur+h->offset);
			h->_free(alter); 
			h->nodecount --;
	    }
    }
	free(h->table);
	free(h);
}

/**************************************************************************************************/
void hashtable_traverse(hashtable_t *h,void *m,void (*_callback)(void *,void *))
{
	if(!h) return;
	
	int i;
   	void *cur=NULL;
	unsigned int node_count;	
 	
	node_count = h->nodecount;
	for(i = 0; i < h->tablelen && node_count>0; i++)
    {
    	cur = h->table[i];
		while (NULL != cur)
		{
			_callback(m,cur);//handle each element
		    cur = (void *)*(unsigned long *)(cur+h->offset);
			node_count --;
		}
	}
}

/**************************************************************************************************/
void hashtable_show(hashtable_t *h,void (* _print)(void *))
{
	if(!h)return;
	
	int i;
   	void *cur=NULL;   	
	
	printf("====================table cotents====================\n");
	fprintf(stdout,"(tablelen,loadlimit,nodecount):=[%u,%u,%u]\n",h->tablelen,h->loadlimit,h->nodecount);
	
	for(i = 0; i < h->tablelen; i++)
    {
    	cur = h->table[i];
		if(cur) printf("%-8d:\n",i);
		else printf("%-8d:\n",i);
		while (NULL != cur){
		    _print(cur);
		    cur = (void *)*(unsigned long *)(cur+h->offset);
		}
	}
	printf("\n");
    return;
}

/*end of file*/

