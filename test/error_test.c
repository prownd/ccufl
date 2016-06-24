#include <stdio.h>
#include "error.h"
int main(){
	int errno=0;
	printf("get_errstr:%s\n",get_errstr(errno));
	show_errstr(errno);
	
	errno=-1;
	printf("get_errstr:%s\n",get_errstr(errno));
	show_errstr(errno);
	
	errno=-2;
	printf("get_errstr:%s\n",get_errstr(errno));
	show_errstr(errno);
}
