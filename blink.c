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
#include "common.h"
#include "blink.h"

// blink
void blink(gdelay) {
static int tm_delay = 0;
static int blink_flag = 1;

if (!run) {
    // led on
    digitalWrite(BLINK_LED, 0);
    delay(gdelay);
    return;
    }

if (tm_delay <= BLINK_DELAY && blink_flag) {
    tm_delay++;
    if(tm_delay == BLINK_DELAY) {
	// on
	digitalWrite(BLINK_LED, 0);
	blink_flag = 0;
	}
    }
else if (!blink_flag) {
    tm_delay--;
    if(tm_delay == 0) {
	// off
        digitalWrite(BLINK_LED, 1);
	blink_flag = 1;
	}
    }
    delay(gdelay);
    return;
}
