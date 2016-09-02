
#ifndef _UUID_H_
#define _UUID_H_

/* what follows is a somewhat stripped-down version of the sample
   implementation of UUID generation from RFC 4122.  */

/*
** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
** Digital Equipment Corporation, Maynard, Mass.
** Copyright (c) 1998 Microsoft.
** To anyone who acknowledges that this file is provided "AS IS"
** without any express or implied warranty: permission to use, copy,
** modify, and distribute this file for any purpose is hereby
** granted without fee, provided that the above copyright notices and
** this notice appears in all source code copies, and that none of
** the names of Open Software Foundation, Inc., Hewlett-Packard
** Company, Microsoft, or Digital Equipment Corporation be used in
** advertising or publicity pertaining to distribution of the software
** without specific, written prior permission. Neither Open Software
** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
** Equipment Corporation makes any representations about the
** suitability of this software for any purpose.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>

/* set the following to the number of 100ns ticks of the actual
   resolution of your system's clock */
#define UUIDS_PER_TICK 1024

#include <sys/types.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/sysinfo.h>


/* system dependent call to get the current system time. Returned as
   100ns ticks since UUID epoch, but resolution may be less than
   100ns. */

#define I64(C) C##LL

typedef uint64_t uuid_time_t;

typedef struct {
  char nodeID[6];
} uuid_node_t;

typedef struct {
  uint32_t  time_low;
  uint16_t  time_mid;
  uint16_t  time_hi_and_version;
  uint8_t   clock_seq_hi_and_reserved;
  uint8_t   clock_seq_low;
  uint8_t   node[6];
} uuid_t;


#endif //_UUID_H_
