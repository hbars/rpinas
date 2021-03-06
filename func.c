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
#include "func.h"

void SetPidFile(char* Filename)
{
    FILE* f;

    f = fopen(Filename, "w+");
    if (f)
    {
        fprintf(f, "%u", getpid());
        fclose(f);
    } else {
	lprintf(LOG_ERR, "can't create pid file: %s %s", PID_FILE, strerror (errno));
	closelog();
	exit (EXIT_FAILURE);
    }
}

void handle_signal(int UNUSED(signo))
{
    int ret;
    ledOff(0, DISP_RANGE);
    lcdClear (fd);
    digitalWrite(BLINK_LED, 1);
    if ((unlink (PID_FILE)) == -1) {
	lprintf(LOG_ERR, "unlink: %s %s", PID_FILE, strerror (errno));
    }
    lprintf(LOG_INFO, "nas exit");
    closelog();
    g_quit = 1;
}

int onesec() {
int retval;
struct timeval timeout = {1, 0};
    retval = select(1, NULL, NULL, NULL, &timeout);
    return (retval);
}

void LCDprintIfInfo (char *ifname, char *ip, float rx_speed, float tx_speed) {
    lcdClear (fd);
    lcdPosition (fd, 0, 0) ;
    lcdPrintf (fd, "%s%s", ifname, ip) ;
    lcdPosition (fd, 0, 1) ;
    lcdPrintf (fd, "%.1f<>%.1f", (float)rx_speed, (float)tx_speed) ;
}
/////////////////////////////////////////////////////////////
void disp_time () {
char buf [30];
time_t tim = time(NULL);
struct tm *t = localtime (&tim);

  lcdClear (fd);
  sprintf (buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
  lcdPosition (fd, 0, 0);
  lcdPuts (fd, buf);

  sprintf (buf, "%02d/%02d/%02d", t->tm_mday, t->tm_mon + 1, t->tm_year+1900);
  lcdPosition (fd, 0, 1);
  lcdPuts (fd, buf);
}
/////////////////////////////////////////////////////////////
IFINFO *IfInfo (char *ifname) {
struct ifaddrs *ifaddr, *ifa;
int family, s, n;
char host[NI_MAXHOST];
IFINFO *ifinfo = (IFINFO*)malloc(sizeof(IFINFO));
if (!ifinfo) {
    lprintf(LOG_ERR, "Out of memory.");
    exit(EXIT_FAILURE);
}
ifinfo->ip = NULL;

if (getifaddrs(&ifaddr) == -1) {
    lprintf(LOG_ERR, "getifaddrs: %s", strerror (errno));
    exit(EXIT_FAILURE);
    }
/* Walk through linked list, maintaining head pointer so we
  can free list later */
           for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
               if (ifa->ifa_addr == NULL || strcmp(ifa->ifa_name, ifname))
                   continue;
               family = ifa->ifa_addr->sa_family;

               // For an AF_INET* interface address.
               if (family == AF_INET) {
                   s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       lprintf(LOG_ERR, "getnameinfo() failed: %s", gai_strerror(s));
                       exit(EXIT_FAILURE);
                   }
		    ifinfo->ip = (char*)strdup(host);
               }
		    else if (family == AF_PACKET && ifa->ifa_data != NULL) {
			ifinfo->stats = ifa->ifa_data;
            	   }
           }
   freeifaddrs(ifaddr);
return (ifinfo);
}

#define ARRAY_SIZE(ar) (sizeof(ar)/sizeof(ar[0]))

int selfunc () {
int lfunc = ARRAY_SIZE(func)-1;
int s = lfunc;

lcdClear (fd);
lcdPrintf (fd, func[s]);
for(;;) {
    if (buttonRes == LEFT) {
	    s--;
	    s = s < 0 ? lfunc : s;
	    lcdClear (fd);
	    lcdPrintf (fd, func[s]);
	    buttonRes = -1;
        }
    else if (buttonRes == RIGHT) {
	    s++;
	    s = s > lfunc ? 0 : s;
	    lcdClear (fd);
	    lcdPrintf (fd, func[s]);
	    buttonRes = -1;
        }
    else if (buttonRes == ENTER) {
	    lcdClear (fd);
	    buttonRes = -1;
            return s;
	}
    (void)blink();
    }
}

// Yes or No LCD set 
int YorN (int x, int y) {
int ret = LEFT;

lcdPosition (fd, x+1, y);
lcdCursorBlink (fd, 1);

for(;;) {
    if (buttonRes == LEFT) {
        lcdPosition (fd, x, y);
        lcdPrintf (fd, "N");
        buttonRes = -1;
        ret = LEFT;
	}
    else if (buttonRes == RIGHT) {
        lcdPosition (fd, x, y);
        lcdPrintf (fd, "Y");
        buttonRes = -1;
        ret = RIGHT;
	}
    else if (buttonRes == ENTER) {
        lcdCursorBlink (fd, 0);
        buttonRes = -1;
	return ret;
	}
    (void)blink();
    }
}

void disp_uptime(){
struct tm *realtime;
time_t realseconds;

while (buttonRes == -1) {
/* first get the current time */
    time(&realseconds);
    realtime = localtime(&realseconds);
    lcdClear(fd);
    lcdPrintf(fd, "%02d:%02d:%02d",
	realtime->tm_hour, realtime->tm_min, realtime->tm_sec);
    lcdPosition(fd, 0, 1);
    lcdPrintf(fd, "%s", sprint_uptime());
    (void)blink();
    }
}

void disp_iwinfo(){
int disp_flag = 1;

lcdClear(fd);
IFINFO *ifinfo = IfInfo(WLANx);
if(ifinfo->ip == NULL){
    free (ifinfo);
    lcdPrintf(fd, "No interface");
    lcdPosition(fd, 0, 1);
    lcdPrintf(fd, "%s", WLANx);
    return;
}

lcdPrintf(fd, "ID:%s", getiwinfo(WLANx, IW" %s link", ssid));
lcdPosition(fd, 0, 1);
lcdPrintf(fd, "SQ:%s", getiwinfo(WLANx, IW" %s link", sig_level));

while (buttonRes == -1) {
    if (disp_flag) {
	    disp_flag = 0;
	    lcdClear(fd);
	    lcdPrintf(fd, "ID:%s", getiwinfo(WLANx, IW" %s link", ssid));
	    lcdPosition(fd, 0, 1);
	    lcdPrintf(fd, "SIG:%s", getiwinfo(WLANx, IW" %s link", sig_level));
	}
    else if (!disp_flag) {
	    lcdClear(fd);
	    lcdPrintf(fd, "RA: %s", getiwinfo(WLANx, IW" %s link", bitrate));
	    lcdPosition(fd, 0, 1);
	    lcdPrintf(fd, "TXP:%s", getiwinfo(WLANx, IW" %s info", txpower));
	    disp_flag = 1;
    }
    (void)blink();
    }
}

// UP or DOWN LCD set 
int UPorDOWN (int x) {
int ret = UP;

lcdPosition (fd, x, 0);
lcdCursorBlink (fd, 1);

for(;;) {
    if (buttonRes == UP) {
        lcdPosition (fd, x, 0);
        buttonRes = -1;
        ret = UP;
	}
    else if (buttonRes == DOWN) {
        lcdPosition (fd, x, 1);
        buttonRes = -1;
        ret = DOWN;
	}
    else if (buttonRes == ENTER) {
        lcdCursorBlink (fd, 0);
        buttonRes = -1;
        return ret;
	}
    (void)blink();
    }
}

void ledOff (int min, int max) {
int i;

for (i = max; i >= min; --i)
    {
      softPwmWrite (DISP_LED, i);
      delay (10);
    }
}

void ledOn (int min, int max) {
int i;

for (i = min; i <= max; ++i)
    {
      softPwmWrite (DISP_LED, i);
      delay (10);
    }
}


/*
 * keys handlers
 *********************************************************************************
 */
void selectInterrupt (void) {
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (ENTER))
    buttonRes = ENTER;
 }
bounceCounter = time_now;
}

void resetInterrupt (void) {
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (SELECT))
    buttonRes = SELECT;
 }
bounceCounter = time_now;
}

void leftInterrupt (void) {
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (LEFT))
    buttonRes = LEFT;
 }
bounceCounter = time_now;
}

void rightInterrupt (void) {
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (RIGHT))
    buttonRes = RIGHT;
 }
bounceCounter = time_now;
}

void upInterrupt (void) {
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (UP))
    buttonRes = UP;
 }
bounceCounter = time_now;
}

void downInterrupt (void) {
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (DOWN))
    buttonRes = DOWN;
 }
bounceCounter = time_now;
}

