#ccufl
c common use function library  
  
  
#compile and install  
make  
make install  
  
  
#example  
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
found a bug? have an awesome idea? post on this github page or mail to me	
email  :  <545751287@qq.com> 
