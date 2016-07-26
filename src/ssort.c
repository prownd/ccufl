#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void quick_sort_partition(void * base,size_t left,size_t right,size_t size,int(* compare)(const void *,const void *)){
	size_t p=(left+right)/2;
	void * pivot=malloc(size);
	memcpy(pivot,base+p*size,size);
	size_t i,j;
	for(i=left,j=right;i<j;)
	{
		while(!(i>p||compare(pivot,base+i*size)<0))
				i++;
		if(i<p)
		{
			memcpy(base+p*size,base+i*size,size);
			p=i;
		}
		while(!(j<=p||compare(base+j*size,pivot)<0))
				j--;
		if(p<j)
		{
			memcpy(base+p*size,base+j*size,size);
			p=j;
		}
	}
	memcpy(base+p*size,pivot,size);
	free(pivot);
	if(p-left>1)
			quick_sort_partition(base,left,p-1,size,compare);
	if(right-p>1)
			quick_sort_partition(base,p+1,right,size,compare);
}

void quick_sort(void* base,size_t nmemb,size_t size ,int(*compare)(const void *,const void*)){
	quick_sort_partition(base,0,nmemb-1,size,compare);
}
