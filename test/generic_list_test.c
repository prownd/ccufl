/****************************************************************
 * This file is part of the ccufl project.                      *
 * @license:    GPLv3                                           *
 * @file:       generic_list_test.h                                  *
 * @version:    1.0                                             *
 * @author:     Han Jinpeng <hanjinpeng127@gmail.com>           *
 * @date:       2025-2-16                                       *
 * @website:    https://github.com/prownd/ccufl                 *
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "generic_list.h"

typedef struct Node_t {
    struct list_head  list;
	int num;
} Node;

int main(int argc,char * argv[]){
	struct list_head nodelist_head;
	Node* iterator = NULL;      // iterator for traverse
    Node* next = NULL;  

    Node * node1 = (Node*)malloc(sizeof(Node));
    node1->num  = 10001;
    node1->list.next = NULL;
    node1->list.prev = NULL;

    Node * node2 = (Node*)malloc(sizeof(Node));
    node2->num  = 10002;
    node2->list.next = NULL;
    node2->list.prev = NULL;

    Node * node3 = (Node*)malloc(sizeof(Node));
    node3->num  = 10003;
    node3->list.next = NULL;
    node3->list.prev = NULL;

    Node * node4 = (Node*)malloc(sizeof(Node));
    node4->num  = 10004;
    node4->list.next = NULL;
    node4->list.prev = NULL;

    Node * node5 = (Node*)malloc(sizeof(Node));
    node5->num  = 10005;
    node5->list.next = NULL;
    node5->list.prev = NULL;

    // initialization list 
    // INIT_LIST_HEAD(&head);
	init_list_head(&nodelist_head);

    // add node data to list
    list_add_tail(&node1->list, &nodelist_head); 
    list_add_tail(&node2->list, &nodelist_head); 
    list_add_tail(&node3->list, &nodelist_head); 
    list_add_tail(&node4->list, &nodelist_head); 
    list_add_tail(&node5->list, &nodelist_head); 

    /*
     * traverse list
     *
     * list_for_each_entry description:
     * arg 1: struct pointer , equal to iterator
     * arg 2: linklist
     * arg 3: name of struct linklist node
     */
    printf("traverse for each list: \n");
    list_for_each_entry(iterator, &nodelist_head, list)
    {
        printf("node num: = %d \n", iterator->num);
    }

    // remove
    list_del(&node1->list);
    printf("traverse for each list: \n");
    list_for_each_entry(iterator, &nodelist_head, list)
    {
        printf("node num = %d \n", iterator->num);
    }

    // replace 
    Node * replace = (Node*)malloc(sizeof(Node));
    replace->num  = 20002;
    replace->list.next = NULL;
    replace->list.prev = NULL;
    list_replace(&node4->list, &replace->list);

    printf("traverse for each list: \n");
    list_for_each_entry(iterator, &nodelist_head, list)
    {
        printf("node num = %d \n", iterator->num);
    }

    // stack variable memory  do not need free; it will automatic
    // delete all node from list
    /*
    printf("delete for each list: \n");
    while (!list_empty(&nodelist_head)) {
        struct Node_t *temp = (struct Node_t *)((char *)nodelist_head.next - (unsigned long) &(((struct Node_t *)0)->list));
        list_del(nodelist_head.next);
        free(temp);
    }
    */

    printf("delete for each list: \n");
    list_for_each_entry_safe(iterator, next, &nodelist_head, list) {
        list_del(&iterator->list);
        free(iterator);
    }
}
