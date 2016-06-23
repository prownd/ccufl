#ifndef _CONN_H_
#define _CONN_H_

#include <time.h>
#include <stdint.h>
#include <unistd.h>

#include "log.h"


typedef enum
{
	CONN_REUSE = 0,
	CONN_NONBLOCK = 1,
	CONN_ALIVE = 2,
	CONN_DEFER = 4,
	CONN_OTHER = 128
} conn_flag_t;

typedef struct
{
	uint32_t sip;
	uint32_t dip;
	uint16_t sport;
	uint16_t dport;
} tuple_t;

typedef struct 
{
	int fd;

	void *arg;
	tuple_t tuple;

	int cs:1;
	int block:1;
	int reuse:1;
	int alive:1;
	int status:1;
} conn_t;

typedef struct g_config_s
{
	char sip[16];
	uint16_t sport;
	char dip[16];
	uint16_t dport;
	uint32_t snd_timeo;
	uint32_t rcv_timeo;
	char host[32];
	uint16_t port;
	char dbname[32];
	char user[32];
	char passwd[32];
	
	uint32_t peer_num;
	tuple_t peers[10];
} g_config_t;

g_config_t g_config;

typedef int (*conn_handle_cb)(conn_t *c, int msec);

/**
 * @brief 创建服务器监听套接字
 * @param conn	封装连接的结构体
 * @param ip	服务器IP地址
 * @param port	服务器监听的端口	
 * @param mask	连接状态掩码
 * @return 
 *		大于0	成功
 *		-1		失败
 */
int conn_open(const char *ip, uint16_t port, int mask);

/**
 * @brief 创建客户端连接套接字
 * @param conn	封装连接的结构体
 * @param ip	服务器端IP地址
 * @param port	服务器端监听的端口
 * @param mask	连接状态掩码
 * @return 
 *		大于0	成功
 *		-1		失败
 */
int conn_peer(const char *ip, uint16_t port, int mask);

/**
 * @brief	检查描述符是否有错误
 *
 */
int conn_check(int fd);
int conn_cycle();

/**
 *
 *
 */
int conn_read(int fd, void *buf, size_t len, int timeout);

/**
 *
 *
 */
int conn_write(int fd, void *buf, size_t len, int timeout);

/**
 *
 *
 */
int conn_recv(conn_t *c, int msec);

/**
 *
 *
 */
int conn_send(conn_t *c, int msec);

#endif /*_CONN_H_*/
