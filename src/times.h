#ifndef _TIMES_H_
#define _TIMES_H_

#include <time.h>
#include <stdlib.h>

/**
 *
 *
 */
int jiffies_update();

/**
 * @brief	get jiffies delta time
 *
 */
uint32_t jiffies_delta();
int ms2tv(int msec, struct timeval *tv);

/**
 * @brief convert local time to string
 * @param	tmbuf	time buffer
 * @param	size	time buffer size 
 * @param	fmt		output time by format
 * @return 
 *			0	success
 */
int time2str(char *tmbuf, size_t size, const char *fmt);

#endif /*_TIMES_H_*/
