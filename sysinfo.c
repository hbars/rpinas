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
 * Copyright (C) 1992-1998 by Michael K. Johnson, johnsonm@redhat.com
 */
#include "common.h"
#include "sysinfo.h"

#define BAD_OPEN_MESSAGE					\
"Error: /proc must be mounted\n"				\
"  To mount /proc at boot you need an /etc/fstab line like:\n"	\
"      /proc   /proc   proc    defaults\n"			\
"  In the meantime, mount /proc /proc -t proc\n"

#define UPTIME_FILE  "/proc/uptime"
static int uptime_fd = -1;

#define BUFSIZE 2048
static char buf[BUFSIZE];

/* This macro opens filename only if necessary and seeks to 0 so
 * that successive calls to the functions are more efficient.
 * It also reads the current contents of the file into the global buf.
 */
#define FILE_TO_BUF(filename, fd) do{				\
    static int local_n;						\
    if (fd == -1 && (fd = open(filename, O_RDONLY)) == -1) {	\
	lprintf(LOG_ERR, BAD_OPEN_MESSAGE);			\
	fflush(NULL);						\
	_exit(102);						\
    }								\
    lseek(fd, 0L, SEEK_SET);					\
    if ((local_n = read(fd, buf, sizeof buf - 1)) < 0) {	\
	lprintf(LOG_ERR, "read error: %s, %s", filename, strerror (errno));\
	fflush(NULL);						\
	_exit(103);						\
    }								\
    buf[local_n] = '\0';					\
}while(0)

/* evals 'x' twice */
#define SET_IF_DESIRED(x,y) do{  if(x) *(x) = (y); }while(0)

/***********************************************************************/
int uptime(double *uptime_secs, double *idle_secs) {
    double up=0, idle=0;
    char *savelocale;

    FILE_TO_BUF(UPTIME_FILE,uptime_fd);
    savelocale = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC,"C");
    if (sscanf(buf, "%lf %lf", &up, &idle) < 2) {
        setlocale(LC_NUMERIC,savelocale);
        lprintf(LOG_ERR, "bad data in " UPTIME_FILE);
        return 0;
    }
    setlocale(LC_NUMERIC,savelocale);
    SET_IF_DESIRED(uptime_secs, up);
    SET_IF_DESIRED(idle_secs, idle);
    return up; /* assume never be zero seconds in practice */
}


char *sprint_uptime(void) {
  int upminutes, uphours, updays;
  int pos=0;
  double uptime_secs, idle_secs;
  static char ubuf[128];

/* read and calculate the amount of uptime */

  uptime(&uptime_secs, &idle_secs);

  updays = (int) uptime_secs / (60*60*24);
//  strcat (ubuf, "up ");
//  pos += 3;
  if (updays)
    pos += sprintf(ubuf + pos, "%d day%s, ", updays, (updays != 1) ? "s" : "");
  upminutes = (int) uptime_secs / 60;
  uphours = upminutes / 60;
  uphours = uphours % 24;
  upminutes = upminutes % 60;
  if(uphours)
    pos += sprintf(ubuf + pos, "%2d:%02d", uphours, upminutes);
  else
    pos += sprintf(ubuf + pos, "%d min", upminutes);
return ubuf;
}

