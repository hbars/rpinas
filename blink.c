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
int blink() {
int ret;

if (!run) {
    // led on
    digitalWrite(BLINK_LED, 0);
    ret = onesec();
    return (ret);
    }

if (!digitalRead(BLINK_LED)) {
    digitalWrite(BLINK_LED, 1);
} else {
    digitalWrite(BLINK_LED, 0);
}
    ret = onesec();
    return (ret);
}
