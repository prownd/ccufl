#include <stdio.h>
#include "htab.h"
static uint32_t ht_hash(const void *key)
{
	uint32_t hash = 5381;
	const char *p = (const char *)key;

	while(*p != '\0')
		hash = ((hash<<5) + hash) + (*p++);

	return hash;
}

static int ht_equal(const void *key1, const void *key2)
{
	return !strcmp(key1, key2);
}

static void ht_delete(void *mem)
{
	if(mem)
		free(mem);
}
int main(){
	int size=12;
	htab_t * ht=htab_init(size,ht_hash,ht_equal,ht_delete);
	char * key1="key1";
	char * value1="value1";
	char * key2="key2";
	char * value2="value2";
	char * getvalue1=NULL;
	char * getvalue2=NULL;
	htab_put(ht,key1,value1);
	htab_put(ht,key2,value2);
	getvalue1=htab_get(ht,key1);
	getvalue2=htab_get(ht,key2);
	printf("the value 1:%s\n",getvalue1);
	printf("the value 2:%s\n",getvalue2);
}
