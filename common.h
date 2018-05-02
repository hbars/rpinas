/*
 * Copyright (c) 2016, Tolya Boirisenkov <hbars@bk.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef COMMON_H
#define COMMON_H

#include "includes.h"

/* define boolean type if needed */
#ifndef HAVE_BOOLEAN
typedef int boolean;
#endif
#ifndef FALSE                   /* in case these macros already exist */
#define FALSE   0               /* values of boolean */
#endif
#ifndef TRUE
#define TRUE    1
#endif

#ifndef UNUSED
#define UNUSED(x) x __attribute__((unused))
#endif

#define lprintf(level, format...)				\
	do {							\
		if (g_verbose || (level != LOG_DEBUG)) {	\
			if (g_daemon || g_syslog)		\
				syslog(level, format);		\
			else					\
				fprintf(stderr, format);	\
		}						\
	} while (0)

void ledOff (int min, int max);
void ledOn (int min, int max);
void disp_time (void);
void rpi_shutdown (void);

void selectInterrupt (void);
void resetInterrupt (void);
void leftInterrupt (void);
void rightInterrupt (void);
void upInterrupt (void);
void downInterrupt (void);

int pwch1 (void);
int pwch2 (void);
void byebye (void);

int YorN (int x, int y);
int UPorDOWN (int x);

volatile int buttonRes;
volatile int bounceCounter;
int fd;
int run;
int g_quit;
int g_daemon;
int g_verbose;
int g_syslog;

struct ifinfo {
    char *ip;
    struct rtnl_link_stats *stats;
};

typedef struct ifinfo IFINFO;

IFINFO *IfInfo (char *ifname);
void LCDprintIfInfo (char *ifname, char *ip, float rx_speed, float tx_speed);

int blink (void);
int selfunc (void);

char *sprint_uptime(void);
void disp_uptime(void);
char *getiwinfo(char *iwname, const char *req, const char *pattern);
void disp_iwinfo(void);

int onesec(void);
char *strmatch_regex(const char *chk_string, const char *pattern);

void handle_signal(int UNUSED(signo));

#endif /* COMMON_H */
