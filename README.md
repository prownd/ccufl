#ccufl
c common use function library,contain some frequently use function. eg :hashtable,linklist,array, dynamic string ,dict and so on.    
一个c的公共函数库 ，包括一些常用的函数和库，例如 hash表，链表，数组，字典，字符串等等    
  
  
#compile and install  
now ,compile
进行编译  



make  
make install  
  
  
  
make dist  
this step can generate dist tar package.  
生成 tar 包  
  
  
make rpm  
generate  rpm package  
生成rpm 二进制包  

  
  
#example
下面是一个hello 例子  
a hello.c example  
```c
#include <stdio.h>
#include "slist.h"
typedef struct node_t {
	int num;
	struct node_t * next;
}node;
void print(void*data){
	printf("the node num:%d\n",((node*)data)->num);
}
int main(int argc,char * argv[]){
	slist_t * slist=NULL;
	node* n1=(node*)malloc(sizeof(node));
	node* n2=(node*)malloc(sizeof(node));
	n1->num=11;
	n1->next=NULL;
	n2->num=22;
	n2->next=NULL;
	slist=slist_create(sizeof(node),NULL,NULL);
	slist_init(slist);
	slist_append(slist,n1);
	slist_append(slist,n2);
	slist_traverse(slist,print,0,slist_count(slist)-1);
	slist_free(slist);
}
```
gcc hello.c -o hello -I/include_path -L/lib_path/ -lccufl 

#contact me
have problem? or have an awesome idea? post on this github issue page ,you can mail to me if you want .   
如果有什么问题，或者你有更好的想法，可以在页面上贴出问题。或者给我发邮件   
Email  :  <545751287@qq.com> 
