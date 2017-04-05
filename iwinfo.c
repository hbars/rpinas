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
#include "iwinfo.h"

char *getiwinfo(char *iwname, const char *req) {
char *savelocale;
FILE *ptr;
char bufcmd[BUFSIZ];
static char buf[BUFSIZ];

sprintf(bufcmd, req, iwname);
savelocale = setlocale(LC_MESSAGES, NULL);
setlocale(LC_MESSAGES,"C");
if ((ptr = popen(bufcmd, "r")) != NULL){
    fgets(buf, BUFSIZ, ptr);
    (void) pclose(ptr);
    }
setlocale(LC_MESSAGES,savelocale);
size_t len = strlen(buf);
if (len ? (buf[len-1] == '\n') : 0)
    buf[len-1] = 0;
return buf;
}
