/****************************************************************
 * This file is part of the ccufl project.                      *
 * @license:    GPLv3                                           *
 * @file:       generic_list.h                                  *
 * @version:    1.0                                             *
 * @author:     Han Jinpeng <hanjinpeng127@gmail.com>           *
 * @date:       2025-2-16                                       *
 * @website:    https://github.com/prownd/ccufl                 *
 ****************************************************************/

#ifndef GENERIC_LIST_H
#define GENERIC_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// custom kernel list impletion, it can by use in application program.

struct list_head {
    struct list_head *next, *prev;
};

static inline void init_list_head(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

/*
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}
*/

#define m_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/*
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 */
#define container_of(ptr, type, member) ({               \
    const typeof(((type *)0)->member) * __mptr = (ptr);  \
    (type *)((char *)__mptr - m_offsetof(type, member)); \
})

/* refer to linux source code: include/linux/list.h */

/*
 * list_entry - get the struct for this entry
 * @ptr:        the &struct list_head pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/*
 * list_for_each_entry  -       iterate over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                   \
    for ((pos) = list_entry((head)->next, typeof(*(pos)), member);   \
         &(pos)->member != (head);                                 \
         (pos) = list_entry((pos)->member.next, typeof(*(pos)), member))

/*
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop counter.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)           \
    for ((pos) = list_entry((head)->next, typeof(*(pos)), member),   \
         (n) = list_entry((pos)->member.next, typeof(*(pos)), member); \
         &(pos)->member != (head);                                 \
         (pos) = (n), (n) = list_entry((n)->member.next, typeof(*(n)), member))


/**
* list_add - add a new entry
* @new: new entry to be added
* @head: list head to add it after
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
static inline void _list_add_origin(struct list_head *new, struct list_head *head)
{
    head->next->prev = new;
    new->next = head->next;
    new->prev = head;
    head->next = new;
}

/*
* Insert a new entry between two known consecutive entries.
*
* This is only for internal list manipulation where we know
* the prev/next entries already!
*/
static inline void _list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
* list_add - add a new entry
* @new: new entry to be added
* @head: list head to add it after
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
static inline void list_add(struct list_head *new, struct list_head *head)
{
    _list_add(new, head, head->next);
}

/**
* list_add_tail - add a new entry
* @new: new entry to be added
* @head: list head to add it before
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    _list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void _list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
* list_del - deletes entry from list.
* @entry: the element to delete from the list.
* Note: list_empty() on entry does not return true after this, the entry is
* in an undefined state.
*/
static inline void list_del(struct list_head *entry)
{
    _list_del(entry->prev, entry->next);
}

/**
* list_replace - replace old entry by new one
* @old : the element to be replaced
* @new : the new element to insert
*
* If @old was empty, it will be overwritten.
*/
static inline void list_replace(struct list_head *old, struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

/**
* list_move - delete from one list and add as another's head
* @list: the entry to move
* @head: the head that will precede our entry
*/
static inline void list_move(struct list_head *list, struct list_head *head)
{
    _list_del(list->prev, list->next);
    list_add(list, head);
}

/**
* list_move_tail - delete from one list and add as another's tail
* @list: the entry to move
* @head: the head that will follow our entry
*/
static inline void list_move_tail(struct list_head *list, struct list_head *head)
{
    _list_del(list->prev, list->next);
    list_add_tail(list, head);
}

/**
* list_empty - tests whether a list is empty
* @head: the list to test.
*/
static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

/**
* list_is_last - tests whether @list is the last entry in list @head
* @list: the entry to test
* @head: the head of the list
*/
static inline int list_is_last(const struct list_head *list, const struct list_head *head)
{
    return list->next == head;
}

#endif // GENERIC_LIST_H
