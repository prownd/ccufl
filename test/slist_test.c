/****************************************************************
 * This file is part of the ccufl project.                      *
 * @license:    GPLv3                                           *
 * @file:       slist_test.c                                    *
 * @version:    1.0                                             *
 * @author:     Han Jinpeng <hanjinpeng127@gmail.com>           *
 * @date:       2015-7-2                                        *
 * @website:    https://github.com/prownd/ccufl                 *
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

typedef struct node_t {
	int num;
	struct node_t * next;
} node;

/*
int cmp(void *source,void*target){
	return ((node*)source)->num-((node*)target)->num;
}*/

void print(void*data){
	printf("the node num:%d\n",((node*)data)->num);
}

int main(int argc,char * argv[]){
	slist_t * slist=NULL;
	node* n1 = (node*)malloc(sizeof(node));
	node* n2 = (node*)malloc(sizeof(node));
	n1->num = 11;
	n1->next = NULL;
	n2->num = 22;
	n2->next = NULL;
	slist = slist_create(sizeof(node),NULL,NULL);
	slist_init(slist);
	slist_append(slist,n1);
	slist_append(slist,n2);
	slist_traverse(slist,print,0,slist_count(slist)-1);
	slist_free(slist);
}
