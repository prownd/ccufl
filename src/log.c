#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "log.h"

log_t *log_create(const char *file, level_t level, time_t duration)
{
	log_t *log;

	log = calloc(sizeof(log_t), 1);
	if(log == NULL){
		perror("log_create");
		return NULL;
	}

	log->name = strdup(file);
	if(log->name == NULL){
		perror("strdup");
		return NULL;
	}

	log->fp = fopen(file, "a");
	if(log->fp == NULL){
		perror("fopen");
		return NULL;
	}
	log->level = level;
	log->duration = duration;

	log->cleanup = log_destroy;
	return log;
}

static int strtime(char *str, size_t size)
{
	time_t local;
	struct tm now;

	time(&local);
	localtime_r(&local, &now);
	strftime(str, size, "%Y-%m-%d %H:%M:%S", &now);

	return 0;
}

int log_inner(log_t *log, const char *file, const long line, const char *fmt, ...)
{
	va_list args;	
	char tmstr[20];

	switch(log->level)
	{
		case LOG_RAW:
			strtime(tmstr, sizeof(tmstr));
			fprintf(log->fp, "[%s]-", tmstr);
			break;
		case LOG_DEBUG:
			strtime(tmstr, sizeof(tmstr));		
			fprintf(log->fp, "[%s]-DEBUG-%d :%s :%ld  ", tmstr, getpid(), file, line);
			break;
		case LOG_WARN:
			strtime(tmstr, sizeof(tmstr));		
			fprintf(log->fp, "[%s]-WARN-%d :%s :%ld  ", tmstr, getpid(), file, line);
			break;
		case LOG_ERROR:
			strtime(tmstr, sizeof(tmstr));		
			fprintf(log->fp, "[%s]-ERROR-%d :%s :%ld  ", tmstr, getpid(), file, line);
			break;
		case LOG_INFO:
			strtime(tmstr, sizeof(tmstr));		
			fprintf(log->fp, "[%s]-INFO-%d :%s :%ld  ", tmstr, getpid(), file, line);
			break;
		default:
			break;
	}

	va_start(args, fmt);
	vfprintf(log->fp, fmt, args);
	va_end(args);
	
	fprintf(log->fp, "\n");

	fflush(log->fp);
	return 0;
}

void log_destroy(log_t *log)
{
	if(log->fp)
		fclose(log->fp);

	if(log->name)
		free(log->name);

	if(log)
		free(log);
}
