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

#ifndef FUNC_H
#define FUNC_H
static const char *func[] = {"SYSTEM", "WLAN INFO", "UPTIME", "MENU EXIT"};

static const char *quality = "iwconfig %s|grep -i 'quality'|\
    sed 's/.*lity=\\([0-9]*\\/[0-9]*\\).*/\\1/'";

static const char *essid = "iwconfig %s|grep -i 'essid'|\
    sed 's/.*\\\"\\(.*\\)\\\"\\ .*/\\1/'";

static const char *rate = "iwconfig %s|grep -i 'rate'|\
    sed 's/.*ate\\:\\(.*\\/s\\)\\ .*/\\1/'";

static const char *level = "iwconfig %s|grep -i 'level'|\
    sed 's/.*evel=\\([0-9]*\\/[0-9]*\\)\\ .*/\\1/'";


#endif /* FUNC_H */
