#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "times.h"

#define tolower(x) ((x >= 'A' && x <= 'Z') ? (x | 0x20) : x)
#define toupper(x) ((x >= 'a' && x <= 'z') ? (x & ~0x20) : x)

int strlower(char *s);
int strupper(char *s);
int strtrip(char *s);
void dec2binstr(uint64_t iDec, char *pBin,int len); 
uint64_t str2bin(char* p, int length);
uint32_t gen_ran(uint32_t low, uint32_t high);

int get_cpus();
int get_sockflag(int sockfd);
void print_hex(const char *data, size_t len);

int set_rlimit(int n);
int set_reuse(int sockfd);
int set_blocking(int sockfd);
int set_nonblocking(int sockfd);
int set_timeout(int sockfd, int sec, int flag);
int set_keepalive(int sockfd);
int set_defer(int sockfd);
int set_affinity(int cpuid, pid_t pid);

#endif /*_UTIL_H_*/
