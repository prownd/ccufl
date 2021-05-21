#ccufl
c common use function library,contain some frequently use function. eg :hashtable,linklist,array, stack,queue,tlv struct, dynamic string ,dict and so on.    
一个c的公共函数库 ，包括一些常用的函数和库，例如 hash表，链表，堆栈，队列，树，tlv结构，数组，字典，字符串等等      
  
  
#compile and install  
now ,compile it

#编译步骤
make  
make install  
install
#安装过程中，会将库和头文件放到/usr/local/lib /usr/local/include 下面 
  
  
make dist  
this step can generate dist tar package.  
生成 tar 包  

make example      
begin compile test example file in directory test.     
编译test目录的example测试源文件。   
    
    
make rpm    
generate  rpm package    
编译，将库文件和头文件打包生成rpm包    
    
     
make deb      
generate debinan package ,fit for debian/ubuntu serial os.  
先编译，然后将库文件和头文件，进行打包封装成deb包。可以直接在debian和ubuntu系列上安装


test目录里面是一些函数和库的具体使用例子。和src内的是对应的。方便更好的入门使用
       
         
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
编译方式：
gcc hello.c -o hello -I/include_path -L/lib_path/ -lccufl 


#contact me
have problem? or have an awesome idea? post on this github issue page ,you can mail to me if you want .   
如果有什么问题，或者你有更好的想法，可以在页面上贴出问题。或者给我发邮件   
Email  :  <545751287@qq.com> 
