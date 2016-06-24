#include "conf.h"
typedef struct configure_t {
	char name[50];
	char passwd[50];
	char ip[16];
	short int port;
}configure;

int config_init(conf_t *conf,configure* myconfig){
	int ret=0;
	if( (ret= conf_getstr(conf, "user", "name", myconfig->name, 50))==-1){
		printf("name not exist\n");
	};

	if((ret = conf_getstr(conf, "user", "passwd", myconfig->passwd,50))==-1){
		printf("passwd not exist\n");
	}
	if((ret = conf_getstr(conf, "host", "ip", myconfig->ip,16))==-1){
		printf("ip not exist\n");
	}
	if ((ret = conf_getshort(conf, "host", "port", &(myconfig->port)))==-1){
		printf("port not exist\n");
	}
}
print_config(configure* myconfig){
	printf("[user]\n");
	printf("name:%s\n",myconfig->name);
	printf("passwd:%s\n",myconfig->passwd);
	printf("[host]\n");
	printf("ip:%s\n",myconfig->ip);
	printf("port:%d\n",myconfig->port);
}

int main(int argc,char * argv[]){
	char * filename="./config.ini";
	int size=100;
	conf_t *conf=NULL;
	configure myconfig; 
	conf=conf_create(filename,size);
	conf_load(conf);
	config_init(conf,&myconfig);
	print_config(&myconfig);	
	conf_destroy(conf);
	return 0;
}


