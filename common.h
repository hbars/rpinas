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

#define VERSION "1.2"

void ledOff (int fd, int min, int max);
void ledOn (int fd, int min, int max);
void disp_time (int fd);
void rpi_shutdown (int fd);

void selectInterrupt (void);
void resetInterrupt (void);
void leftInterrupt (void);
void rightInterrupt (void);
void upInterrupt (void);
void downInterrupt (void);

int pwch1 (int fd);
int pwch2 (int fd);
void byebye (int fd);

int YorN (int fd, int x, int y);
int UPorDOWN (int fd, int x);

volatile int buttonRes;
volatile int bounceCounter;
int fd;
int run;

struct ifinfo {
    char *ip;
    struct rtnl_link_stats *stats;
};

typedef struct ifinfo IFINFO;

IFINFO *IfInfo (char *ifname);
void LCDprintIfInfo (char *ifname, char *ip, float rx_speed, float tx_speed, int fd);

int blink (void);
int selfunc (int fd);

char *sprint_uptime(void);
void disp_uptime(void);
char *getiwinfo(char *iwname, const char *req);
void disp_iwinfo(void);

int onesec(void);

#endif /* COMMON_H */
