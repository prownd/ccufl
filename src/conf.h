#ifndef _CONF_H_
#define _CONF_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "htab.h"
#include "global.h"

typedef struct conf_s conf_t;
typedef void (*conf_cleanup_cb)(conf_t *conf);


extern g_config_t g_config;

struct conf_s 
{
	FILE *fp;
	char *name;
	htab_t *table;

	conf_cleanup_cb cleanup;
};

conf_t *conf_create(const char *confname, size_t size);

int conf_load(conf_t *conf);

void *conf_get(conf_t *conf, void *section,void *key);
int conf_getstr(conf_t *conf, void *section, void *key, char *value, uint32_t len);
int conf_getshort(conf_t *conf, void *section, void *key, uint16_t *value);
int conf_getint(conf_t *conf, void *section, void *key, int *value);
int conf_getuint(conf_t *conf, void *section, void *key, uint32_t *value);
void conf_destroy(conf_t *conf);

#endif /*_CONF_H_*/
