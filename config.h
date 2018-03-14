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

#ifndef CONFIG_H
#define CONFIG_H

/*
#define XX wPi  // GPIO BCM [board pin]
see "gpio readall"
*/

// LCD pins configuration
#define RS 11  // GPIO 7 [26]
#define EN 10  // GPIO 8 [24]
#define DB4 14 // GPIO 11[23]
#define DB5 13 // GPIO 9 [21]
#define DB6 12 // GPIO 10 [19]
#define DB7 6  // GPIO 25 [22]

// Keys configuration
#define ENTER 5 // GPIO 24 [18]
#define SELECT 4  // GPIO 23 [16]
#define LEFT 19  // GPIO 30 [53]
#define RIGHT 20 // GPIO 31 [54]
#define UP 18    // GPIO 29 [52]
#define DOWN 17  // GPIO 28 [51]

// blink
#define BLINK_LED 0 // GPIO 17 [11]

// LCD led configuration
#define DISP_LED 1 // GPIO 18 [12]
#define DISP_RANGE 70 // max bright
#define DISP_FLASH_TIME 25 // display flash time

#define BOUNCE_DELAY 150 // buttons bounce delay
#define BOUNCE_RELEASE 40 // buttons release delay

// interfaces
#define ETHx	"eth0"
#define WLANx	"wlan0"

#define	DTIME 3 // disp date sec
#define ETHTIME 5 // ethernet in sec
#define WLANTIME 7 // wlan in sec

#endif /* CONFIG_H */
