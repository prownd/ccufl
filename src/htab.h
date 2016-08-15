/**
 * @file htab.h
 * @time 2015-9-12
 * @author hanjinpeng127@163.com
 * @version 1.0
 */

#ifndef _HTAB_H_
#define _HTAB_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct htab_s htab_t;
typedef uint32_t (*htab_hash_cb)(const void *key);
typedef int (*htab_equal_cb)(const void *key1, const void *key2);
typedef void (*htab_delete_cb)(void *key);

typedef struct htab_entry_s
{
	uint32_t hash;
	void *key;
	void *value;

	struct htab_entry_s *prev;
	struct htab_entry_s *next;
} htab_entry_t;

struct htab_s
{
	size_t size;
	htab_entry_t **bucket;

	size_t items;

	htab_hash_cb hash;
	htab_equal_cb equal;
	htab_delete_cb del;
};

htab_t *htab_init(size_t size, htab_hash_cb hash, htab_equal_cb equal, htab_delete_cb del);

int htab_put(htab_t *table, void *key, void *value);

void *htab_get(htab_t *table, void *key);

void htab_remove(htab_t *table, void *key);

void htab_destroy(htab_t *table);

htab_entry_t *htab_begin(htab_t *table);

htab_entry_t *htab_next(htab_t *table, htab_entry_t *entry);

#define htab_foreach(table, entry)	\
	for(entry=htab_begin(table); entry; entry=htab_next(table, entry))

#endif /*_HTAB_H_*/
