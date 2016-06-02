#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "util.h"

int strlower(char *s)
{
	for(int i=0; i<strlen(s); i++)
		s[i] = tolower(s[i]);

	return 0;
}

int strupper(char *s)
{
	for(int i=0; i<strlen(s); i++)
		s[i] = toupper(s[i]);

	return 0;
}

int strtrip(char *s)
{
	uint32_t len;

	len = strlen(s);

	if(len > 1 && s[len-1]==' ')
		s[len-1] = '\0';

	for(char *p=s,*q=s; *p!='\0'; p++, q++)
	{
		if(*p == ' '){
			while(*q == ' ')
				q++;
		}

		*p = *q;
		if(*q == '\0')
			break;
	}

	return 0;
}

char * strcatquot(char * buf1,char * buf2){
	int len=strlen(buf1);
	printf("%d\n",len);
	int i=0;
	*buf2='\'';
	for(i=0;i<len;i++){
		*(buf2+i+1)=*(buf1+i);
	}
	*(buf2+i+1)='\'';
	return buf2;
}

uint64_t str2bin(char* p, int length){
		uint64_t num=0;
		int	i=0;
		for(i=0;i<length;i++){
			if(*(p+length-i-1) == '1'){
				num += (uint64_t)pow(2, i);
			}
		}
		return num;
}

void dec2binstr(uint64_t iDec, char *pBin,int len) { 
	uint64_t  temp=iDec; 
	int i=len-1; 
	while(i>=0){ 
		pBin[i]=((temp&1)?'1':'0'); 
		temp=temp>>1; 
		i--; 
	} 
} 

uint32_t gen_ran(uint32_t low, uint32_t high)
{
	if(low > high)
		return -1;

	return rand()%(high-low+1) + low;
}





int get_cpus()
{
	return sysconf(_SC_NPROCESSORS_ONLN);
}



int get_sockflag(int sockfd)
{
	int flag = -1;

	while((flag=fcntl(sockfd, F_GETFL, 0)) == -1)
	{
		if(errno == EINTR)
			continue;
		return -1;
	}

	return flag;
}


void print_hex(const char *data, size_t len)
{
	unsigned char c;

	for(size_t i=0; i<len; i++)
	{
		c = data[i];
		printf("%.2x ", c);
		if(i % 16 == 15)
			putchar('\n');
	}
	putchar('\n');
}

int set_rlimit(int n)
{
	struct rlimit limit;

	limit.rlim_cur = n;
	limit.rlim_max = n;

	if(setrlimit(RLIMIT_NOFILE, &limit) == -1){
		perror("setrlimit");
		return -1; 
	}   

	return 0;
}

int set_reuse(int sockfd)
{
	int reuse = 1;

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
		perror("setsockopt");
		return -1;
	}

	return 0;
}

int set_blocking(int sockfd)
{
	int val = 0;
	int flags, mask;

	while((flags=fcntl(sockfd, F_GETFL, 0)) == -1)
	{
		if(errno == EINTR)
			continue;
		return -1;
	}
	mask = flags;
	if(!(mask & O_NONBLOCK)){
		return 0;
	}
	flags &= ~O_NONBLOCK;

	while((val=fcntl(sockfd, F_SETFL, flags)) == -1)
	{
		if(errno == EINTR)
			continue;
		return -1;
	}

	return val;
}

int set_nonblocking(int sockfd)
{
	int opts;

	opts = fcntl(sockfd, F_GETFL);
	if(opts < 0){   
		perror("fcntl");
		return -1; 
	}   

	opts = opts | O_NONBLOCK;
	if(fcntl(sockfd, F_SETFL, opts) < 0){   
		perror("fcntl");
		return -1; 
	}   

	return 0;
}

int set_timeout(int sockfd, int sec, int flag)
{
	struct timeval tv;

	tv.tv_sec = sec;
	tv.tv_usec = 0;

	if(flag == 0){
		if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(struct timeval)) != 0){
			perror("setsockopt");
			return -1;
		}
	}else if(flag == 1){
		if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(struct timeval)) != 0){
			perror("setsockopt");
			return -1;
		}
	}else{
		return -1;
	}

	return 0;
}

int set_keepalive(int sockfd)
{
	int alive = 1;

	if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (const char *)&alive, sizeof(alive)) != 0){
		perror("setsockopt");
		return -1;
	}

	return 0;
}

int set_defer(int sockfd)
{
	int delay = 3;

	if(setsockopt(sockfd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &delay, sizeof(delay)) != 0){
		perror("setsockopt");
		return -1;
	}

	return 0;
}

int set_affinity(int cpuid, pid_t pid)
{
	cpu_set_t mask;

	CPU_ZERO(&mask);
	CPU_SET(cpuid, &mask);

	if(sched_setaffinity(pid, sizeof(cpu_set_t), &mask) == -1){
		perror("sched_setaffinity");
		return -1;
	}

	return 0;
}
