#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "conf.h"
#include "util.h"
#include "htab.h"

#define BUFSIZE 128
#define DEBUG_CONF 1

static uint32_t ht_hash(const void *key)
{
	uint32_t hash = 5381;
	const char *p = (const char *)key;

	while(*p != '\0')
		hash = ((hash<<5) + hash) + (*p++);

	return hash;
}

static int ht_equal(const void *key1, const void *key2)
{
	return !strcmp(key1, key2);
}

static void ht_delete(void *mem)
{
	if(mem)
		free(mem);
}

conf_t *conf_create(const char *confname, size_t size)
{
	conf_t *conf;

	conf = calloc(sizeof(conf_t), 1);
	if(conf == NULL){
		//TODO:
		return NULL;
	}
	conf->name = strdup(confname);
	if(conf->name == NULL){
		//TODO:
		return NULL;
	}

	conf->fp = fopen(conf->name, "r");
	if(conf->fp == NULL){
		perror("fopen");
		free(conf);
		return NULL;
	}

	conf->table = htab_init(size, ht_hash, ht_equal, ht_delete);
	if(conf->table == NULL){
		fprintf(stderr, "[Error]-fail in htab_init.\n");
		free(conf);
		return NULL;
	}

	conf->cleanup = conf_destroy;
	return conf;
}

int conf_load(conf_t *conf)
{
	char *line = NULL;
	ssize_t n = 0;
	size_t len = 0;

	char *key, *value, *perm, *section=NULL;

	while((n=getline(&line, &len, conf->fp)) != -1)
	{
		if(n == 0){
			continue;
		}else if(line[0] == '#' || line[0] == ';'){
			continue;
		}else if(line[n-1] == '\n'){
			n --;
			line[n] = '\0';
		}else{
			//TODO:
		}

		key = calloc(sizeof(char), BUFSIZE);	
		value = calloc(sizeof(char), BUFSIZE);	
		perm = calloc(sizeof(char), BUFSIZE);

		if(line[0]=='[' && line[n-1]==']'){
			section = calloc(sizeof(char), BUFSIZE);
			sscanf(line, "[%[^]]", section);	
			strtrip(section);
			strlower(section);
		}else if(sscanf (line, "%[^=] = \"%[^\"]\"", key, value) == 2
				|| sscanf (line, "%[^=] = '%[^\']'", key, value) == 2
				|| sscanf (line, "%[^=] = %[^;#]", key, value) == 2){
			/* Usual key=value, with or without comments */
			strtrip(key);
			strlower(key);
			strtrip(value);
			if(!strcmp(value, "\"\"") || (!strcmp(value, "''")))
				value[0] = '\0';

			sprintf(perm, "%s:%s", section, key);
			free(key);
			htab_put(conf->table, perm, value);
		}else if(sscanf(line, "%[^=] = %[;#]", key, value)==2
				|| sscanf(line, "%[^=] %[=]", key, value) == 2){
			/*  
			 * Special cases:
			 * key=
			 * key=;
			 * key=#
			 */
			strtrip(key);
			strlower(key);
			value[0] = '\0';

			sprintf(perm, "%s:%s", section, key);
			free(key);
			htab_put(conf->table, perm, value);
		}else{
			//TODO:
			free(key);
			free(value);
			free(perm);
		}
	}

	return 0;
}

void *conf_get(conf_t *conf, void *section, void *key)
{
	void *value;
	void *seckey;

	seckey = calloc(sizeof(char), strlen(section)+strlen(key)+2);
	if(seckey == NULL){
		perror("calloc");
		return NULL;
	}
	sprintf(seckey, "%s:%s", (char *)section, (char *)key);
	strlower(seckey);

	value = htab_get(conf->table, seckey);
	if(value == NULL){
		//TODO:
	}

	free(seckey);
	return value;
}

int conf_getstr(conf_t *conf, void *section, void *key, char *value, uint32_t len)
{
	void *str;

	if(value == NULL)
		return -1;

	str = conf_get(conf, section, key);
	if(str == NULL)
		return -1;

	if(strlen(str) >= len)
		return -1;

	strncpy(value, str, len);

	return 0;
}

int conf_getshort(conf_t *conf, void *section, void *key, uint16_t *value)
{
	void *str;

	if(value == NULL)
		return -1;

	str = conf_get(conf, section, key);
	if(str == NULL)
		return -1;

	*value = (uint16_t)atoi(str);

	return 0;
}

int conf_getint(conf_t *conf, void *section, void *key, int *value)
{
	void *str;

	if(value == NULL)
		return -1;

	str = conf_get(conf, section, key);
	if(str == NULL)
		return -1;

	*value = atoi(str);

	return 0;
}

int conf_getuint(conf_t *conf, void *section, void *key, uint32_t *value)
{
	void *str;

	if(value == NULL)
		return -1;

	str = conf_get(conf, section, key);
	if(str == NULL)
		return -1;

	*value = atoi(str);

	return 0;
}

#if 0
int config_init_1(conf_t *conf)
{
	void *value;

	if(conf == NULL){
		return -1;
	}

	value = conf_get(conf, "Generic", "sip");
	if(value == NULL){
		return -1;
	}else{
		strncpy(g_config.sip, (char *)value, 16);
	}

	value = conf_get(conf, "Generic", "sport");
	if(value == NULL){
		return -1;
	}else{
		g_config.sport = (uint16_t)atoi(value);
	}

	value = conf_get(conf, "Database", "host");
	if(value == NULL){
		return -1;
	}else{
		strncpy(g_config.host, (char *)value, 32);
	}

	value = conf_get(conf, "Database", "port");
	if(value == NULL){
		return -1;
	}else{
		g_config.port = (uint16_t)atoi(value);
	}

	value = conf_get(conf, "Database", "dbname");
	if(value == NULL){
		return -1;
	}else{
		strncpy(g_config.dbname, (char *)value, 32);
	}

	value = conf_get(conf, "Database", "user");
	if(value == NULL){
		return -1;
	}else{
		strncpy(g_config.user, (char *)value, 32);
	}

	value = conf_get(conf, "Database", "passwd");
	if(value == NULL){
		return -1;
	}else{
		strncpy(g_config.passwd, (char *)value, 32);
	}

	
	value = conf_get(conf, "Peers", "dip");
	if(value == NULL){
		return -1; 
	}else{
		strncpy(g_config.peer_ip, (char *)value, 16);
	}   

	value = conf_get(conf, "Peers", "dport");
	if(value == NULL){
		return -1; 
	}else{
		g_config.peer_port = (uint16_t)atoi(value);
	}   
	return 0;
}
#endif

int config_init(conf_t *conf){
	int err = 0;
	char section[64];
	err = conf_getstr(conf, "Generic", "sip", g_config.sip, 16);
	err = conf_getshort(conf, "Generic", "sport", &g_config.sport);
	//err = conf_getuint(conf, "Generic", "WORKERS", &g_config.workers);
	err = conf_getuint(conf, "Generic", "snd_timeo", &g_config.snd_timeo);
	err = conf_getuint(conf, "Generic", "rcv_timeo", &g_config.rcv_timeo);
	err = conf_getuint(conf, "Generic", "peer_num", &g_config.peer_num);
	err = conf_getstr(conf, "Database", "host", g_config.host,32);
	err = conf_getshort(conf, "Database", "port", &g_config.port);
	err = conf_getstr(conf, "Database", "dbname", g_config.dbname,32);
	err = conf_getstr(conf, "Database", "user", g_config.user,32);
	err = conf_getstr(conf, "Database", "passwd", g_config.passwd,32);
	for(int i=0; i<g_config.peer_num; i++){
		sprintf(section, "Peer%d", i);
		err = conf_getstr(conf, section, "dip", g_config.peers[i].ip, 16);
		err = conf_getshort(conf, section, "dport", &g_config.peers[i].port);
	}
	if(err == -1)
		return -1;
	conf_destroy(conf);
	return 0;
}

void conf_destroy(conf_t *conf){
	if(conf->table)
		htab_destroy(conf->table);
	if(conf->fp)
		fclose(conf->fp);
	if(conf->name)
		free(conf->name);
	if(conf)
		free(conf);
}
