#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_
#include <stdlib.h>
#include <stdio.h>

#undef TRUE
#define TRUE 1

#undef FALSE
#define FALSE 0

#undef boolean
#define boolean short unsigned int

typedef struct arraylist_s arraylist;

void arraylist_free(arraylist* array);

arraylist* arraylist_create(const boolean (*equals)(const void* obj1, const void* obj2));

boolean arraylist_add(arraylist *array, void* object);

boolean arraylist_remove(arraylist *array, const void * object);

boolean arraylist_contains(const arraylist *array, const void * object);

int arraylist_index_of(const arraylist* array, const void* object);

boolean arraylist_is_empty(const arraylist *array);

int arraylist_size(const arraylist *array);

void* arraylist_get(const arraylist *array, const int index);

void arraylist_clear(arraylist *array);

void arraylist_sort(const arraylist *array, const int (*compare)(const void* obj1, const void * obj2));

#endif /* _ARRAYLIST_H_ */
