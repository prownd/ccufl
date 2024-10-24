/*************************************************************************
> File Name: adlist_mytest.cpp
> Author: han
> Mail: hanjinpeng127@163.com 
> Created Time: 2016年07月19日 星期二 14时47分10秒
************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "adlist.h"

typedef struct data_t {
    int id;
    char name[40];
}data;

int main() {
    list *l = listCreate();

    data* d1=(data*)malloc(sizeof(data));
    d1->id=11;
    strcpy(d1->name,"tom");
    listAddNodeHead(l, (void*)d1); 

    data* d2=(data*)malloc(sizeof(data));
    d2->id=12;
    strcpy(d2->name,"jake");
    listAddNodeHead(l, (void*)d2);

    data* d3=(data*)malloc(sizeof(data));
    d3->id=13;
    strcpy(d3->name,"bob");
    listAddNodeTail(l, (void*)d3);

    int i=0;
    listNode* tmp=NULL;
    for(i=0;i<l->len;i++){
        tmp=listIndex(l,i);
        printf("id:%d,name:%s\n",((data*)tmp->value)->id,((data*)tmp->value)->name);
    }

    return 0;
}


