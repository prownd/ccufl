#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "ssort.h"

int compare_int(const void * pv1,const void * pv2){
	return *(const int *)pv1-*(const int * )pv2;
}

int compare_string(const void * pv1,const void* pv2){
	const char*psz1=*(const char**)pv1;
	const char*psz2=*(const char**)pv2;
	return strcmp(psz1,psz2);
}

struct Persion{
	char name[128];
	int age;
};

int compare_persion(const void * pv1,const void* pv2){
	const struct Persion s1=*(const struct Persion *)pv1;
	const struct Persion s2=*(const struct Persion *)pv2;
	int name_cmp=strcmp(s1.name,s2.name);
	return name_cmp?name_cmp:s2.age-s1.age;
}

//#define BUBBLE_SORT
//gcc  ssort_test.c  -DPRINT_FUNCTION_NAME -DBUBBLE_SORT
int main(){
		int i;
        int na[]={34,56,17,88,41,69,24,73,81,6};
#if defined BUBBLE_SORT
		bubble_sort(na,sizeof(na)/sizeof(na[0]),sizeof(na[0]),compare_int);
#elif defined QUICK_SORT
		quick_sort(na,sizeof(na)/sizeof(na[0]),sizeof(na[0]),compare_int);
#endif
		for(i=0;i<sizeof(na)/sizeof(na[0]);i++)
				printf("%d ",na[i]);
		printf("\n");
		
        char * pa[]={"beijing","shanghai","washingtons","seattle"};
#if defined BUBBLE_SORT
		bubble_sort(pa,sizeof(pa)/sizeof(pa[0]),sizeof(pa[0]),compare_string);
#elif defined QUICK_SORT
		quick_sort(pa,sizeof(pa)/sizeof(pa[0]),sizeof(pa[0]),compare_string);
#endif
		for(i=0;i<sizeof(pa)/sizeof(pa[0]);i++)
				printf("%s ",pa[i]);
		printf("\n");

		struct Persion sa[]={{"jime",22},{"peter",23},{"hanks",20},{"linda",26}};
#if defined BUBBLE_SORT
		bubble_sort(sa,sizeof(sa)/sizeof(sa[0]),sizeof(sa[0]),compare_persion);
#elif defined QUICK_SORT
		quick_sort(sa,sizeof(sa)/sizeof(sa[0]),sizeof(sa[0]),compare_persion);
#endif
		for(i=0;i<sizeof(sa)/sizeof(sa[0]);i++)
				printf(" %s,%d ",sa[i].name,sa[i].age);
		printf("\n");
		
        return 0;
}
