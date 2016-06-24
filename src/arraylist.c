/**
 *
 *author:hanjinpeng
 *email:hanjinpeng127@163.com
 *date:2016-6-23
 */
#include <string.h>
#include "arraylist.h"
 
#define ARRAYLIST_INITIAL_CAPACITY 10
#define ARRAYLIST_CAPACITY_DELTA 10
 
static const size_t object_size = sizeof(void*);
 
 
struct arraylist_s {
  int current_capacity;
  void **data;
  int size;
  const boolean (*equals)();
};
 
 
static void *checked_malloc(const size_t size);
 
 
void arraylist_free(arraylist* array)
{
  free(array->data);
  free(array);
}
 
arraylist* arraylist_create(const boolean (*equals)(const void* obj1, const void* obj2))
{
  arraylist* array;
 
#ifdef GOK_DEBUG
  array = malloc(sizeof(struct arraylist_s));
#else
  array = checked_malloc(sizeof(struct arraylist_s));
#endif
  array->current_capacity = ARRAYLIST_INITIAL_CAPACITY;
#ifdef GOK_DEBUG
  array->data = malloc(objectsize * array->current_capacity);
#else
  array->data = checked_malloc(object_size * array->current_capacity);
#endif
  array->size = 0;
  array->equals = equals;
 
  return array;
}
 
boolean arraylist_add(arraylist* array, void* object)
{
  int old_size = arraylist_size(array);
  int new_capacity;
  void**new_data;
 
  (array->size)++;
  if (old_size == array->current_capacity)
    {
      new_capacity = array->current_capacity + ARRAYLIST_CAPACITY_DELTA;
#ifdef GOK_DEBUG
      new_data = malloc(objectsize * new_capacity);
#else
      new_data = checked_malloc(object_size * new_capacity);
#endif
      memcpy(new_data, array->data, object_size * old_size);
      free(array->data);
      (array->data) = new_data;
      array->current_capacity = new_capacity;
    }
  (array->data)[old_size] = object;
  return TRUE;
}
 
boolean arraylist_remove( arraylist* array, const void* object)
{
  int length = arraylist_size(array);
  int last_index = length - 1;
  int new_size, new_capacity;
  int index;
 
  for (index = 0; index < length; index++)
    {
      if ((*array->equals)(arraylist_get(array, index), object))
	{
	  (array->size)--;
	  if (index < last_index)
	    {
	      memmove(array->data + index, array->data + index + 1, object_size * (last_index - index));
	      new_size = array->size;
	      new_capacity = array->current_capacity - ARRAYLIST_CAPACITY_DELTA;
	      if (new_capacity > new_size)
		{
		  array->data = realloc(array->data, object_size * new_capacity);
		  array->current_capacity = new_capacity;
		}
	    }
	  return TRUE;
	}
    }
  return FALSE;
}
 
boolean arraylist_contains(const arraylist* array, const void* object)
{
  return (arraylist_index_of(array, object) > -1);
}
 
int arraylist_index_of(const arraylist* array, const void* object)
{
  int length = arraylist_size(array);
  int index;
 
  for (index = 0; index < length; index++)
    {
      if ((*array->equals)(arraylist_get(array, index), object))
	{
	  return index;
	}
    }
  return -1;
}
 
boolean arraylist_is_empty(const arraylist* array)
{
  return (0 == arraylist_size(array));
}
 
int arraylist_size(const arraylist* array)
{
  return array->size;
}
 
void* arraylist_get(const arraylist* array, const int index)
{
  return array->data[index];
}
 
void arraylist_clear(arraylist* array)
{
  array->data = realloc(array->data, object_size * ARRAYLIST_INITIAL_CAPACITY);
  array->current_capacity = ARRAYLIST_INITIAL_CAPACITY;
  array->size = 0;
}
 
void arraylist_sort(const arraylist* array, const int (*compare)(const void* obj1, const void* obj2))
{
  qsort(array->data,
	arraylist_size(array),
	sizeof(void*),
	(int (*)())compare);
}
 
static void *checked_malloc(const size_t size)
{
  void *data;
 
  data = malloc(size);
  if (data == NULL)
    {
      fprintf(stderr, "\nOut of memory.\n");
      fflush(stderr);
      exit(EXIT_FAILURE);
    }
 
  return data;
}
