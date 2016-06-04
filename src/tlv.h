#ifndef _TLV_H_
#define _TLV_H_

#include <stdint.h>
#include <string.h>

#pragma pack(1)
typedef struct
{
	uint8_t tag;
	uint32_t len;
	char value[0];
} tlv_node_t;
#pragma pack()

/**
 * @brief	获取tlv结构体中value部分长度
 * @param	tlv_node	tlv结构体节点
 * @return	tlv结构体中value的长度
 */
size_t tlv_node_len(tlv_node_t *tlv_node);

/**
 * @brief	将buf中内容编码为tlv节点
 * @param	tag		tlv节点的标识
 * @param	buf		需要编码的内容，即tlv节点中value的内容
 * @param	len		tlv节点中value部分的内容长度
 * @return
 *			node	tlv节点指针
 *			NULL	错误
 */
tlv_node_t *tlv_node_encode(uint8_t tag, const char *buf, size_t len);

/**
 *
 *
 */
int tlv_node_decode(tlv_node_t *tlv_node, char *buf, size_t len);

/**
 *
 *
 */
void tlv_node_free(tlv_node_t *tlv_node);

#endif /*_TLV_H_*/
