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

// global circle delay
#define G_DELAY 10
// display next info
#define DISP_NEXT_INFO_DELAY G_DELAY*5.5

// blink
#define BLINK_LED 0 // GPIO 17 [11]
#define BLINK_DELAY G_DELAY*5

// LCD led configuration
#define DISP_LED 1 // GPIO 18 [12]
#define DISP_RANGE 70 // max bright
#define DISP_FLASH_TIME 15 // display flash time
#define DISP_BEFORE_NEXT_IN_SEC 2 // display time before next instance

#define BOUNCE_DELAY 150 // buttons bounce delay
#define BOUNCE_RELEASE 40 // buttons release delay

// interfaces
#define ETHx	"eth0"
#define WLANx	"wlan0"

#endif /* CONFIG_H */
