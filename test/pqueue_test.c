#include <stdio.h>
#include "pqueue.h"
typedef struct node_t {
	int num;
	int seq;
}node;
int main(int argc,char * argv[]){
	node * node1=(node*)malloc(sizeof(node));
	node * node2=(node*)malloc(sizeof(node));
	node* node_tmp=NULL;
	node1->num=11;
	node1->seq=1;
	node2->num=22;
	node2->seq=2;
	pqueue_t * pq=pqueue_create(20);
	pqueue_addone(pq,node1);
	pqueue_addone(pq,node2);
	node_tmp=pqueue_getone(pq);
	printf("node num:%d, seq:%d\n",node_tmp->num,node_tmp->num);
	pqueue_free(pq);
	return 0;
}
