/*************************************************************************
> File Name: sds_mytest.c
> Author: han
> Mail: hanjinpeng127@163.com 
> Created Time: 2016年07月19日 星期二 17时17分49秒
************************************************************************/

#include<stdio.h>
#include "sds.h"

int main(){

    char hello[20]={"hello,world!"};
    printf("hello:%s\n",hello);
    
    printf("str new\n");
    sds str=sdsnew(hello);
    printf("str:%s\n",str);
    printf("str len:%d\n",sdslen(str));
    printf("str avail:%d\n",sdsavail(str));

    printf("str cat\n");
    sds newstr=sdscat(str,"this is strcat content");
    printf("str:%s\n",str);
    printf("str:%s\n",newstr);
    printf("str len:%d\n",sdslen(newstr));
    printf("str avail:%d\n",sdsavail(newstr));

    printf("str cpy\n");
    char* teststr="test a string";
    sdscpy(newstr,teststr);
    printf("str:%s\n",newstr);
    printf("str len:%d\n",sdslen(newstr));
    printf("str avail:%d\n",sdsavail(newstr));


    printf("trim char \n");
    sdstrim(newstr,"g");
    printf("str:%s\n",newstr);
    
    printf("sds range  \n");
    sdsrange(newstr,1,3);
    printf("str:%s\n",newstr);
    
    printf("sds toupper  \n");
    sdstoupper(newstr);
    printf("str:%s\n",newstr);
    
    printf("sds tolower  \n");
    sdstolower(newstr);
    printf("str:%s\n",newstr);
    
    printf("sds cmp  \n");
    sds cmpstr=sdsnew("est");
    printf("str:%d\n",sdscmp(newstr,cmpstr));
    return 0;
}



