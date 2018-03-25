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

static const char *ssid = "\\s(SSID:)\\s(.+)";
static const char *signal = "\\s(signal:)\\s(.+)";
static const char *bitrate = "\\s(tx bitrate:)\\s(.+)";
static const char *txpower = "\\s(txpower)\\s(.+)";


#endif /* FUNC_H */
