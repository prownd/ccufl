#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "error.h"

const char *g_errstr[] = { 
	"CF_RET_SUCCESS",
	"CF_RET_EPARAM",
	"CF_RET_EBODYLEN",
	"CF_RET_READ",
	"CF_RET_READHEAD",
	"CF_RET_WRITE",
	"CF_RET_WRITEHEAD",
	"CF_RET_PEERCLOSE",
	"CF_RET_ETIMEDOUT",
	"CF_RET_EMAGICNUM",
	"CF_RET_UNKNOWN",
};

const char *get_errstr(int ret)
{
	if(ret>0 || ret<1-(int)(sizeof(g_errstr) / sizeof(char *)))
		return g_errstr[-CF_RET_UNKNOWN];

	return g_errstr[-ret];
}

int show_errstr(int ret)
{
	const char *str;

	str = get_errstr(ret);
	fprintf(stderr, "[Error]-%s\n", str);

	return 0;
}
