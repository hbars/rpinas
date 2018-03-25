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

char *getiwinfo(char *iwname, const char *req, const char *pattern) {
char *savelocale;
FILE *ptr;
char bufcmd[BUFSIZ];
char buf[BUFSIZ];
static char *res = NULL;

sprintf(bufcmd, req, iwname);
savelocale = setlocale(LC_MESSAGES, NULL);
setlocale(LC_MESSAGES,"C");
if ((ptr = popen(bufcmd, "r")) != NULL) {
    while ((fgets(buf, BUFSIZ, ptr) != NULL && !feof(ptr)) 
    && (res = strmatch_regex(buf, pattern)) == NULL) {
	if( ferror(ptr) ) { perror("fgets"); break; }
	}
    (void) pclose(ptr);
    }
setlocale(LC_MESSAGES,savelocale);
if (res) {
    size_t len = strlen(res);
    if (len ? (res[len-1] == '\n') : 0)
        res[len-1] = 0;
    }
return res;
}
