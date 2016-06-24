#include <stdio.h>
#include "stack.h"
typedef struct node_t {
	int num;
	struct node_t * next;
}node;
int main(int argc,char * argv[]){
	stack_t * st=NULL;
	node* n1=(node*)malloc(sizeof(node));
	node* n2=(node*)malloc(sizeof(node));
	n1->num=11;
	n1->next=NULL;
	n2->num=22;
	n2->next=NULL;
	st=stack_create(sizeof(node),NULL);
	stack_init(st);
	stack_push(st,n1);
	stack_push(st,n2);
	node* tmp=(node*)stack_pop(st);
	printf("node num:%d\n",tmp->num);
	free(tmp);
	tmp=(node*)stack_top(st);
	printf("node num:%d\n",tmp->num);
	stack_free(st);
}
