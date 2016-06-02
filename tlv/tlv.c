#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tlv.h"

size_t tlv_node_len(tlv_node_t *tlv)
{
	size_t len = 0;

	len = tlv->len;
	len += sizeof(tlv_node_t);

	return len;
}

tlv_node_t *tlv_node_encode(uint8_t tag, const char *buf, size_t len)
{
	tlv_node_t *node;

	node = calloc(sizeof(tlv_node_t) + len, 1);
	if(node == NULL){
		return NULL;
	}

	node->tag = tag;
	node->len = len;
	memcpy(node->value, buf, len);

	return node;
}

int tlv_node_decode(tlv_node_t *tlv_node, char *buf, size_t buflen)
{
	uint8_t len;

	len = tlv_node->len;
	if(len > buflen){
		return -1;
	}

	memcpy(buf, tlv_node->value, tlv_node->len);

	return tlv_node->len;
}

void tlv_node_free(tlv_node_t *tlv_node)
{
	if(tlv_node)
		free(tlv_node);
}
