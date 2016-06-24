/*************************************************************************
	> File Name: arraylist_test.c
	> Author: hanjinpeng
	> Mail: hanjinpeng127@163.com 
	> Created Time: 2016年06月23日 星期四 19时53分22秒
 ************************************************************************/

#include<stdio.h>
#include "arraylist.h"
typedef struct node_t {
	int num;
	int seq;
}node;
int main(int argc,char argv[]){
	arraylist* arr=arraylist_create(NULL);
	node* tmp=NULL;
	node* n1=malloc(sizeof(node));
	n1->num=11;
	n1->seq=1;
	arraylist_add(arr,n1);
	tmp=arraylist_get(arr,0);
	printf("data num:%d,seq:%d\n",tmp->num,tmp->seq);
	arraylist_clear(arr);
	return 0;
}
