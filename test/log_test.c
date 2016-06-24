#include<stdio.h>
#include "log.h"
int main(int argc,char *argv[]){
	log_t * log=NULL;
	enum level level_seq=LOG_DEBUG;
	time_t duration=100;
	char * filename="./log.txt";
	log=log_create(filename,level_seq,duration);
	log_debug(log,"this is debug level log");
	log_debug(log,"may be some debug log print");
	log_destroy(log);
}
