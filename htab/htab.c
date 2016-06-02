#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "htab.h"

#define DEBUG_HTAB 1

static htab_t *htab_create(size_t size)
{
	htab_t *table;

	table = calloc(sizeof(htab_t), 1);
	if(table == NULL){
		perror("calloc");
		return NULL;
	}

	table->bucket = calloc(sizeof(htab_entry_t *), size);
	if(table->bucket == NULL){
		perror("calloc");
		free(table);
		return NULL;
	}

	return table;
}

htab_t *htab_init(size_t size, htab_hash_cb hash, htab_equal_cb equal, htab_delete_cb delete)
{
	htab_t *table;

	table = htab_create(size);
	if(table == NULL){
		//TODO:
		return NULL;
	}

	table->size = size;
	table->items = 0;
	table->hash = hash;
	table->equal = equal;
	table->del = delete;

	return table;
}

static int htab_resize(htab_t *table)
{
	size_t size;
	htab_entry_t **bucket, *p, *q;

	size = 2 * table->size;
	bucket = calloc(sizeof(htab_entry_t *), size);
	if(bucket == NULL){
		perror("calloc");
		return -1;
	}

	for(size_t i=0; i<table->size; i++)
	{
		size_t j = 0;
		for(p = (table->bucket)[i]; p; p = q)
		{
			q = p->next;

			p->prev = NULL;
			p->next = NULL;

			j = p->hash % size;
			if(bucket[j]){
				bucket[j]->prev = p;
				p->next = bucket[j];
			}
			bucket[j] = p;
		}
	}

	free(table->bucket);
	table->bucket = bucket;
	table->size = size;

	return 0;
}

int htab_put(htab_t *table, void *key, void *value)
{
	int status;
	uint32_t index;
	htab_entry_t *p = NULL;

	index = table->hash(key) % table->size;
	for(p=(table->bucket)[index]; p; p=p->next)
	{
		if(table->equal(key, p->key))
			break;
	}

	if(p){
		if(table->del){
			table->del(p->key);
			table->del(p->value);
		}

		p->key = key;
		p->value = value;
		return 0;
	}else{
		if(table->items * 1.3 > table->size){
			status = htab_resize(table);
			if(status){
				return -1;
			}
		}

		p = calloc(sizeof(htab_entry_t), 1);
		if(p == NULL){
			perror("calloc");
			return -1;
		}

		p->hash = table->hash(key);
		p->key = key;
		p->value = value;
		p->prev = NULL;
		p->next = NULL;

		index = p->hash % table->size;
		if((table->bucket)[index]){
			(table->bucket)[index]->prev = p;
			p->next = (table->bucket)[index];
		}

		(table->bucket)[index] = p;
		table->items ++;
	}

	return 0;
}

void *htab_get(htab_t *table, void *key)
{
	uint32_t index;
	htab_entry_t *p = NULL;

	index = table->hash(key) % table->size;
	for(p=(table->bucket)[index]; p; p=p->next)
	{
		if(table->equal(key, p->key))
			return p->value;
	}

	return NULL;
}

void htab_remove(htab_t *table, void *key)
{
	uint32_t index;
	htab_entry_t *p = NULL;

	index = table->hash(key) % table->size;
	for(p=(table->bucket)[index]; p; p=p->next)
	{
		if(table->equal(key, p->key))
			break;
	}

	if(!p){
		return ;
	}

	if(table->del){
		table->del(p->key);
		table->del(p->value);
	}

	if(p->next){
		p->next->prev = p->prev;
	}
	if(p->prev){
		p->prev->next = p->next;
	}else{
		index = table->hash(key) % table->size;
		table->bucket[index] = p->next;
	}

	free(p);
	table->items --;

	return ;
}

void htab_destroy(htab_t *table)
{
	htab_entry_t *p, *q;

	for(size_t i=0; i<table->size; i++)
	{
		for(p=(table->bucket)[i]; p; p=q)
		{
			q = p->next;
			if(table->del){
				table->del(p->key);
				table->del(p->value);
			}

			free(p);
		}
		(table->bucket)[i] = NULL;
	}

	table->items = 0;

	if(table->bucket)
		free(table->bucket);
	if(table)
		free(table);
}

htab_entry_t *htab_begin(htab_t *table)
{
	htab_entry_t *p = NULL;

	for(size_t i=0; i<table->size; i++)
	{
		for(p=table->bucket[i]; p; p=p->next)
			if(p)
				return p;
	}

	return NULL;
}

htab_entry_t *htab_next(htab_t *table, htab_entry_t *entry)
{
	size_t index;

	if(entry->next)
		return entry->next;

	index = entry->hash % table->size;

	for(size_t i=index+1; i<table->size; i++)
	{
		if((table->bucket)[i])
			return (table->bucket)[i];
	}

	return NULL;
}
