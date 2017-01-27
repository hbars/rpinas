/*
 * RPi NAS - Default include files
 * Copyright (c) 2016, Tolya Borisenkov <hbars@bk.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This header file is included into all C files so that commonly used header
 * files can be selected with OS specific ifdef blocks in one place instead of
 * having to have OS/C library specific selection in many files.
 */

#ifndef INCLUDES_H
#define INCLUDES_H

/* Include possible build time configuration before including anything else */
#include "config.h"

#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
//#include <math.h>
#include <time.h>
#include <sys/time.h>
/*
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <ctype.h>
#include <signal.h>
*/
// getifaddrs
#include <ifaddrs.h>
#include <netdb.h>
#include <linux/if_link.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// RPi
#include <wiringPi.h>
#include <lcd.h>
#include <softPwm.h>

#endif /* INCLUDES_H */
