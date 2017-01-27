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
/*
unsigned long ROL (unsigned long a, int offset)
{
       return a << offset | a >> (32 - offset);
}
*/
void LCDprintIfInfo (char *ifname, char *ip, float rx_speed, float tx_speed, int fd) {

    lcdClear (fd);
    lcdPosition (fd, 0, 0) ;
    lcdPrintf (fd, "%s%s", ifname, ip) ;
    lcdPosition (fd, 0, 1) ;
    lcdPrintf (fd, "%.1f<>%.1f", (float)rx_speed, (float)tx_speed) ;
//printf("%s%s - %.1f<>%.1f\n", ifname, ip,(float)rx_speed, (float)tx_speed);
}
/////////////////////////////////////////////////////////////
time_t disp_time (int fd) {

  char buf [30] ;
  struct tm *t ;
  time_t tim ;

  tim = time (NULL) ;
  t = localtime (&tim) ;

  lcdClear (fd);
  sprintf (buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec) ;
  lcdPosition (fd, 0, 0) ;
  lcdPuts (fd, buf) ;

  sprintf (buf, "%02d/%02d/%02d", t->tm_mday, t->tm_mon + 1, t->tm_year+1900) ;
  lcdPosition (fd, 0, 1) ;
  lcdPuts (fd, buf) ;

return (tim);
}
/////////////////////////////////////////////////////////////
IFINFO *IfInfo (char *ifname)
{
struct ifaddrs *ifaddr, *ifa;
int family, s, n;
char host[NI_MAXHOST];
IFINFO *ifinfo = (IFINFO*)malloc(sizeof(IFINFO));

ifinfo->ip = NULL;

    if (getifaddrs(&ifaddr) == -1) {
           perror("getifaddrs");
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
                       printf("getnameinfo() failed: %s\n", gai_strerror(s));
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

// Yes or No LCD set 
int YorN (int fd, int x, int y) {
    int ret = LEFT;

    lcdPosition (fd, x+1, y) ;
    lcdCursorBlink (fd, 1) ;
    for(;;){
        if (buttonRes == LEFT) {
	    lcdPosition (fd, x, y) ;
	    lcdPrintf (fd, "N") ;
	    buttonRes = -1;
	    ret = LEFT;
        }
    else if (buttonRes == RIGHT) {
	    lcdPosition (fd, x, y) ;
	    lcdPrintf (fd, "Y") ;
	    buttonRes = -1;
	    ret = RIGHT;
        }
    else if (buttonRes == SELECT) {
	    lcdCursorBlink (fd, 0) ;
	    buttonRes = -1;
            return ret;
	}
    delay(100);
    }

}

// UP or DOWN LCD set 
int UPorDOWN (int fd, int x) {
    int ret = UP;

    lcdPosition (fd, x, 0) ;
    lcdCursorBlink (fd, 1) ;
    for(;;){
        if (buttonRes == UP) {
        lcdPosition (fd, x, 0) ;
        buttonRes = -1;
        ret = UP;
	}
    else if (buttonRes == DOWN) {
        lcdPosition (fd, x, 1) ;
        buttonRes = -1;
        ret = DOWN;
	}
    else if (buttonRes == SELECT) {
        lcdCursorBlink (fd, 0) ;
        buttonRes = -1;
        return ret;
	}
delay(100);
    }

}

void ledOff (int fd, int min, int max) {

int i;

for (i = max ; i >= min ; --i)
    {
      softPwmWrite (DISP_LED, i) ;
      delay (10) ;
    }
}

void ledOn (int fd, int min, int max) {

int i;

for (i = min ; i <= max ; ++i)
    {
      softPwmWrite (DISP_LED, i) ;
      delay (10) ;
    }
}


/*
 * keys handlers
 *********************************************************************************
 */
void selectInterrupt (void)
{
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (SELECT))
    buttonRes = SELECT;
 }
bounceCounter = time_now;
}

void resetInterrupt (void)
{
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (RESET))
    buttonRes = RESET;
 }
bounceCounter = time_now;
}

void leftInterrupt (void)
{
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (LEFT))
    buttonRes = LEFT;
 }
bounceCounter = time_now;
}

void rightInterrupt (void)
{
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (RIGHT))
    buttonRes = RIGHT;
 }
bounceCounter = time_now;
}

void upInterrupt (void)
{
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (UP))
    buttonRes = UP;
 }
bounceCounter = time_now;
}

void downInterrupt (void)
{
int time_now=millis();

if ((time_now - bounceCounter) >= BOUNCE_DELAY){
  delay (BOUNCE_RELEASE);
  if (!digitalRead (DOWN))
    buttonRes = DOWN;
 }
bounceCounter = time_now;
}

