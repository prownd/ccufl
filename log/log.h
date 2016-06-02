#ifndef _LOG_H_
#define _LOG_H_

#include <time.h>
#include <stdio.h>

typedef enum level level_t;
typedef struct log_s log_t;
typedef void (*log_cleanup_cb)(log_t *log);

enum level 
{
	LOG_RAW=0,
	LOG_DEBUG=1,
	LOG_WARN=2,
	LOG_ERROR=3,
	LOG_INFO=4
};

struct log_s
{
	FILE *fp;
	char *name;

	level_t level;
	time_t duration;

	log_cleanup_cb cleanup;
};

#define log_debug(log, ...) \
	log_inner(log, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(log, ...) \
	log_inner(log, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(log, ...) \
	log_inner(log, __FILE__, __LINE__, __VA_ARGS__)
#define log_profile(log, ...) \
	log_inner(log, __FILE__, __LINE__, __VA_ARGS__)

log_t *log_create(const char *file, level_t level, time_t duration);

int log_inner(log_t *log, const char *file, const long line, const char *fmt, ...);

void log_destroy(log_t *log);

#endif /*_LOG_H_*/
