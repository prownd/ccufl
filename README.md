#ccufl
c common use function library  
  
  
#compile and install  
make  
make install  
  
  
#example  
a hello.c example  
```c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
static const int ITEM_COUNT = 5;
typedef struct _value_{
	unsigned int key;
	unsigned short value;
	struct _value_ *next;
}value_t;
static unsigned int hashkey(void *key){
    value_t *k = (value_t*)key;
	register unsigned int hash=0;
    hash = (((k->key<< 17) | (k->key>> 15)) ^ k->value);
	return (hash);
}
static void value_print(void *key){
	value_t *k = (value_t*)key;
	printf("\t[%u,%hu]\n",k->key,k->value);
}
int main(int argc, char **argv){
	int i=0;
	void *found=NULL;
	value_t *k=NULL,v;
	hashtable_t *h=NULL;
	h=(hashtable_t*)hashtable_create(ITEM_COUNT,sizeof(value_t),NULL,hashkey,NULL);
	for (i = 0; i < ITEM_COUNT; i++){
		k = (value_t *)calloc(1,sizeof(value_t));
		if (NULL == k) 	return 1;
		k->key= 0xcfccee40 + i;
		k->value= 0xcf0cee67 - (5 * i);
		if(hashtable_insert(h,k))  break;	
	}
	hashtable_show(h,value_print);
	hashtable_free(h);
	h=NULL;
	return 0;	
}
```
gcc hello.c -o hello -I/include_path -L/lib_path/ -lccufl 

