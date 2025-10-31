# Description
c common use function library, including some commonly used functions and libraries, such as hash table, linked list, stack, queue, tree, TLV structure, array, dictionary, string, etc.

  


# Build 
compile:

```bash
make  
make install  
install
```

Note: During the installation process, the library and header files will be placed under /usr/local/lib /usr/local/include



```bash
make dist 
```


it can generate dist tar package.  



### Generate example 
```bash
make example      
```

Compile test example file in directory test.     
    

### rpm package

```bash
make rpm    
```

Generate  rpm package    


### deb package
```bash  
make deb
```

Generate deb package file, suitable for Ubuntu/Debian system

Note: First compile, then package the library and header files into a deb package. This can be installed directly on Debian and Ubuntu systems. The test directory contains some specific examples of how to use the functions and libraries. These correspond to those in src, making it easier to get started.       
         

### Example code
Here is an example of hello.c
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
Compile cmd：

```bash
gcc hello.c -o hello -I/include_path -L/lib_path/ -lccufl 
```



# Contact me
If you encounter bug issues or have better suggestions while using it.
Please report issue questions or submit PR for modification and optimization.
You can also send me an email to contact and communicate with me
Welcome feedback and contributions, thank you! 

Email  :  <hanjinpeng127@163.com> 
