/*************************************************************************
> File Name: dict_test.c
> Author: hanjinpeng
> Mail: hanjinpeng127@163.com 
> Created Time: 2016年07月22日 星期五 17时26分53秒
************************************************************************/

#include<stdio.h>
#include "dict.h"

int main(){
    //dictType type={dictGenHashFunction,NULL,NULL,NULL,NULL,NULL};
    int i=0,len=0;
    dictEntry* entry=NULL;
    dictIterator* iter=NULL;

    dict * dict=dictCreate(&dictTypeHeapStringCopyKey,NULL);
    char *key[6]={"key_11","key_22","key_33","key_44","key_55","key_66"};
    char *value[6]={"value_11","value_22","value_33","value_44","value_55","value_66"};
    len=sizeof(key)/sizeof(char*);

    //add key:value pair into hashtable
    for(i=0;i<len;i++)
        dictAdd(dict,key[i],value[i]);

    //find 
    entry=dictFind(dict,key[0]);
    printf("entry->key : %s , entry->val :%s\n",entry->key,entry->val);

    
    //iterator query  , get entry by ht->table[index] 
    iter=dictGetIterator(dict);
    
    entry=dictNext(iter);
    printf("entry->key : %s , entry->val :%s\n",entry->key,entry->val);
    
    entry=dictNext(iter);
    printf("entry->key : %s , entry->val :%s\n",entry->key,entry->val);
    
    //print status
    dictPrintStats(dict);
    
    //free 
    dictRelease(dict);

    return 0;
}
