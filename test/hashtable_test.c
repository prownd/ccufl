/**
 * @file hashtable_test.c
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

static const int ITEM_COUNT = 8;

typedef struct _value_{
	unsigned int sip;
	unsigned short sport;
	unsigned int dip;
	unsigned short dport;
	
	struct _value_ *next;
}value_t;


static unsigned int hashkey(void *key)
{
    value_t *k = (value_t*)key;
	register unsigned int hash=0;
    hash = (((k->sip<< 17) | (k->sip>> 15)) ^ k->dip) +(k->sport* 17) + (k->dport* 13 * 29);
	return (hash);
}

static int equalkeys(void *k1, void *k2)
{
    return memcmp(k1,k2,sizeof(value_t)-sizeof(value_t*));
}

static void value_print(void *key)
{
	value_t *k = (value_t*)key;
	printf("\t[%u,%hu,%u,%hu]\n",k->sip,k->sport,k->dip,k->dport);
}
/**************************************************************************************************/
int main(int argc, char **argv)
{
	int i=0;
	void *found=NULL;
	value_t *k=NULL,v;
	hashtable_t *h=NULL;
	
	
	//h=(hashtable_t*)hashtable_create(1,sizeof(value_t),NULL,hashkey,equalkeys);
	h=(hashtable_t*)hashtable_create(ITEM_COUNT,sizeof(value_t),NULL,hashkey,equalkeys);
	if (NULL == h){
		fprintf(stderr,"[error]:can't create the hashtable!\n");
		exit(1);
	}
	printf("h:%p,table:%p\n",h,h->table);
	printf("tablelen:%u,limit:%u\n",h->tablelen,h->loadlimit);
	
	//----------Insertion
	for (i = 0; i < 3*ITEM_COUNT; i++)
	{
		k = (value_t *)calloc(1,sizeof(value_t));
		if (NULL == k) {
		    fprintf(stderr,"[error]:ran out of memory allocating a key\n");
		    return 1;
		}
		k->sip= 0xcfccee40 + i;
		k->dip= 0xcf0cee67 - (5 * i);
		k->sport= 22 + (7 * i);
		k->dport= 5522 - (3 * i);
		
		if(hashtable_insert(h,k)){
			fprintf(stdout,"[error]:insert\n");
			break;
		}
	}
	printf("After insertion, hashtable contains %u items.\n",hashtable_count(h));
	hashtable_show(h,value_print);
	
	#if 1
	//--------remove
	for (i = 0; i < ITEM_COUNT; i++)
    {
    	memset(&v,0,sizeof(value_t));
        v.sip= 0xcfccee40 + i;
        v.dip= 0xcf0cee67 - (5 * i);
        v.sport= 22 + (7 * i);
        v.dport= 5522 - (3 * i);
        
        if(hashtable_remove(h,&v)) {
            fprintf(stdout,"[error]: [%u,%hu,%u,%hu] not found for removal\n",
					v.sip,v.sport,v.dip,v.dport);
        }		
    }
	printf("After removal, hashtable contains %u items.\n",hashtable_count(h));	
	hashtable_show(h,value_print);
	#endif
	
	#if 0
	//--------search	
    for (i = 0; i < 2*ITEM_COUNT; i++)
    {
    	memset(&v,0,sizeof(value_t));
        v.sip= 0xcfccee40 + i;
        v.dip= 0xcf0cee67 - (5 * i);
        v.sport= 22 + (7 * i);
        v.dport= 5522 - (3 * i);
        
        if(NULL == (found = hashtable_search(h,&v))) {
            fprintf(stdout,"[Tip]: [%u,%hu,%u,%hu] not found\n",
					v.sip,v.sport,v.dip,v.dport);
        }
    }
	#endif
		
	//--------free
	printf("tablelen:%u\n",h->tablelen);
	hashtable_free(h);h=NULL;
	
	
	return 0;	
}
/*end of file*/




