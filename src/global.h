#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string.h>
#include <stdint.h>


typedef struct
{
	char ip[16];
	uint16_t port;
} ip_tuple_t;


typedef struct g_config_s
{
	char sip[16];
	uint16_t sport;
	//char dip[16];
	//uint16_t dport;
	uint32_t snd_timeo;
	uint32_t rcv_timeo;
	char host[32];
	uint16_t port;
	char dbname[32];
	char user[32];
	char passwd[32];
	
	uint32_t peer_num;
	ip_tuple_t peers[10];
}g_config_t;

typedef struct conn_s conn_t;

#endif /*_GLOBAL_H_*/
