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
#include "nas.h"

int main (int argc, char *argv[]) {
int disp_r = DISP_RANGE;
time_t tim;
time_t old_sec = 0;
time_t ttim = 0;
int disp_flag = 0;
IFINFO *ifinfo;
float rx_speed = 0, tx_speed = 0;
unsigned int rx_old_eth = 0;
unsigned int tx_old_eth = 0;
unsigned int bin = 0;
unsigned int rx_old_wlan = 0;
unsigned int tx_old_wlan = 0;
unsigned int bout = 0;

struct timeval t_now = {0, 0};
struct timeval t_last_eth = {0, 0};
struct timeval t_last_wlan = {0, 0};

unsigned int counter = 0;
int c = 0;
double timedelta;

int f;

//printf ("RPi NAS v %s\n", VERSION) ;

if (wiringPiSetup () == -1) {
    fprintf (stderr, "GPIO Setup failed! %s\n", strerror (errno));
    exit (EXIT_FAILURE);
  }

// configure buttons pins
pinMode (ENTER, INPUT) ;
pinMode (SELECT, INPUT) ;
pinMode (LEFT, INPUT) ;
pinMode (RIGHT, INPUT) ;
pinMode (UP, INPUT) ;
pinMode (DOWN, INPUT) ;
pinMode(BLINK_LED, OUTPUT); // set mode to output
pullUpDnControl (ENTER, PUD_UP) ;
pullUpDnControl (SELECT, PUD_UP) ;
pullUpDnControl (LEFT, PUD_UP) ;
pullUpDnControl (RIGHT, PUD_UP) ;
pullUpDnControl (UP, PUD_UP) ;
pullUpDnControl (DOWN, PUD_UP) ;

if (wiringPiISR (ENTER, INT_EDGE_FALLING, &selectInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (SELECT, INT_EDGE_FALLING, &resetInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (LEFT, INT_EDGE_FALLING, &leftInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (RIGHT, INT_EDGE_FALLING, &rightInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (UP, INT_EDGE_FALLING, &upInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (DOWN, INT_EDGE_FALLING, &downInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }

// configure LCD pins
fd = lcdInit (2,16,4, RS,EN, DB4,DB5,DB6,DB7,0,0,0,0) ;
if (fd == -1)
  {
    fprintf (stderr, "lcdInit 1 failed: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }

if (softPwmCreate (DISP_LED, 0, DISP_RANGE) < 0)
  {
    fprintf (stderr, "Unable to setup PWM: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }

lcdClear (fd);
lcdPosition (fd, 0, 0) ;
lcdPrintf (fd, " RPi NAS v%s", VERSION) ;
lcdPosition (fd, 0, 1) ;
lcdPrintf (fd, " was loaded ...") ;
ledOn(fd, 0, DISP_RANGE);
delay (1000) ;
lcdClear (fd);
lcdPosition (fd, 0, 0) ;

run = 1;
disp_r = DISP_RANGE;
gettimeofday(&t_last_eth, NULL);
t_last_wlan=t_last_eth;

tim = time (NULL);
ttim = tim + DISP_FLASH_TIME;
counter = tim + DISP_BEFORE_NEXT_IN_SEC;
buttonRes = -1;
for (;;)
{
    gettimeofday(&t_now, NULL);
    tim = time (NULL) ;
    if (buttonRes != -1 && disp_flag)
	{
	ttim = tim + DISP_FLASH_TIME;
	ledOn(fd, 0, disp_r);
	disp_flag = 0;
	}
    if ((tim > ttim) && !disp_flag && run)
	{
	ledOff(fd, 0, disp_r);
	disp_flag = 1;
	}

// buttons
    switch (buttonRes) {
    case ENTER:
	run = run ? 0:1;
        buttonRes = -1;
//puts("s\n");
    break;
    case UP:
        if (disp_r < DISP_RANGE) {
            disp_r += 5;
	    disp_r = disp_r > DISP_RANGE ? DISP_RANGE : disp_r;
	    softPwmWrite (DISP_LED, disp_r) ;
        }
        buttonRes = -1;
    break;
    case DOWN:
        if (disp_r > 0) {
            disp_r -= 5;
	    disp_r = disp_r < 0 ? 0 : disp_r;
	    softPwmWrite (DISP_LED, disp_r) ;
        }
        buttonRes = -1;
    break;
    case LEFT:
        buttonRes = -1;
        c--;
    break;
    case RIGHT:
        buttonRes = -1;
        c++;
    break;
    case SELECT:
	f=selfunc(fd);
	ledOn(fd, disp_r, disp_r);
	switch (f) {
	    case 0: // SYSTEM
		rpi_shutdown (fd);
	    break;
	    case 1: // WLAN INFO
		disp_iwinfo();
	    break;
	    case 2: // UPTIME
		disp_uptime();
	    break;
	    case 3: // MENU EXIT
	    default:
	    break;
	}
	buttonRes = -1;
    break;
    default:
#define MAX_INST 30 // with default

	if (tim > old_sec) {
	    if (tim > counter && run) {
		counter = tim + DISP_BEFORE_NEXT_IN_SEC;
		c++;
		c = c < MAX_INST ? c : 0;
	    }
	    switch (c) {
	        case 0:
	        case 1:
		    disp_time(fd) ;
	        break;
	        case 2:
	        case 3:
	        case 4:
	        case 5:
	        case 6:
	        case 7:
	        case 8:
	        case 9:
	        case 10:
	        case 11:
	        case 12: //11
		    ifinfo = IfInfo(ETHx);
		    if(ifinfo->ip == NULL){
			c += 11;
			free (ifinfo);
			break;
		    }
		    bin  = ifinfo->stats->rx_bytes + (rx_old_eth & ~0xffffffffULL);
		    bout = ifinfo->stats->tx_bytes + (tx_old_eth & ~0xffffffffULL);
		    if ( bin < rx_old_eth )
		      bin += (1ULL << 32);
		    if ( bout < tx_old_eth )
		      bout += (1ULL << 32);

		    timedelta = (double)(t_now.tv_sec - t_last_eth.tv_sec) +
		    (t_now.tv_usec - t_last_eth.tv_usec)/1.0e+6;

		    rx_speed = (float)((bin - rx_old_eth)*8/timedelta)/1000;
		    tx_speed = (float)((bout - tx_old_eth)*8/timedelta)/1000;
		    LCDprintIfInfo ("E", ifinfo->ip, rx_speed, tx_speed, fd);
		    rx_old_eth = bin;
		    tx_old_eth = bout;
		    t_last_eth = t_now;
		    free (ifinfo->ip);
		    free (ifinfo);

	        break;
	        case 13:
	        case 14:
	        case 15:
	        case 16:
	        case 17:
	        case 18:
	        case 19:
	        case 20:
	        case 21:
	        case 22:
	        case 23:
	        case 24:
	        case 25:
	        case 26:
	        case 27:
	        case 28:
	        case 29: //17
		    ifinfo = IfInfo(WLANx);
		    if(ifinfo->ip == NULL){
			c += 17;
			free (ifinfo);
			break;
		    }
		    bin  = ifinfo->stats->rx_bytes + (rx_old_wlan & ~0xffffffffULL);
		    bout = ifinfo->stats->tx_bytes + (tx_old_wlan & ~0xffffffffULL);
		    if ( bin < rx_old_wlan )
		      bin += (1ULL << 32);
		    if ( bout < tx_old_wlan )
		      bout += (1ULL << 32);

		    timedelta = (double)(t_now.tv_sec - t_last_wlan.tv_sec) +
		    (t_now.tv_usec - t_last_wlan.tv_usec)/1.0e+6;

		    rx_speed = (float)((bin - rx_old_wlan)*8/timedelta)/1000;
		    tx_speed = (float)((bout - tx_old_wlan)*8/timedelta)/1000;
		    LCDprintIfInfo ("W", ifinfo->ip, rx_speed, tx_speed, fd);
		    rx_old_wlan = bin;
		    tx_old_wlan = bout;
		    t_last_wlan = t_now;
		    free (ifinfo->ip);
		    free (ifinfo);
	        break;
	        default:   // MAX_INST
		    c = 0;
	    }
	old_sec = time (NULL) ;
      }
      break;
    }
    blink(G_DELAY);
 }
return (EXIT_SUCCESS) ;
}
