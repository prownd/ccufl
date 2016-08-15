/**
 * @file time.c
 * @time 2015-5-15
 * @author hanjinpeng127@163.com
 * @version 1.1
 */

#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

time_t jiffies = 0;

int jiffies_update()
{
	jiffies = time(NULL);

	return 0;
}

uint32_t jiffies_delta()
{
	uint32_t delta = 0;
	time_t now = time(NULL);

	delta = (uint32_t)((now>jiffies)?(now-jiffies):0);

	return delta;
}


int ms2tv(int msec, struct timeval *tv)
{
	if(tv == NULL){
		return -1;
	}

	tv->tv_sec = msec / 1000;
	tv->tv_usec = (msec % 1000) * 1000;

	return 0;
}

uint32_t get_msec()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_sec*1000 + tv.tv_usec/1000);
}


int time2str(char *tmbuf, size_t size, const char *fmt)
{
	time_t local;
	struct tm now;

	local = time(NULL);
	localtime_r(&local, &now);
	strftime(tmbuf, size, fmt, &now);

	return 0;
}
