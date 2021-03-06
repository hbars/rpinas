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

static void print_help(void)
{
printf("nas v" VERSION " -- nas control for embedded system\n"
	       "\n"
	       "Usage: nas [options]\n"
	       "\n"
	       "  -n, --foreground       Run in foreground, do not detach from controlling terminal\n"
	       "  -s, --syslog           Use syslog for logging, even if running in the foreground\n"
	       "  -v, --verbose          Verbose messages\n"
	       "  -h, --help             This help text\n"
	       "\n");
}

int main (int argc, char *argv[]) {

int disp_r = DISP_RANGE;
time_t tim;
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

int retval = 0;
double timedelta;

int f; //select swith

unsigned int i = 0; // swich block
unsigned int c = 0; // distlay time counter

struct sigaction sig;
char   *__progname;

char short_options[] = "nsvh";
struct option long_options[] = {
	{ "foreground", 0, 0, 'n' },
	{ "syslog", 0, 0, 's' },
	{ "verbose", 0, 0, 'v' },
	{ "help", 0, 0, 'h' },
	{ NULL, 0, 0, 0 }
};
int option_index = 1;

/* Prevent TERM and HUP signals from interrupting system calls */
sig.sa_handler = handle_signal;
sigemptyset (&sig.sa_mask);
sig.sa_flags = SA_RESTART;
sigaction(SIGTERM, &sig, NULL);
sigaction(SIGHUP, &sig, NULL);

g_daemon = 1;
g_verbose = 0;
g_syslog = 0;

/* Parse commandline options */
while (1) {
	c = getopt_long(argc, argv, short_options, long_options, &option_index);
	if (c == -1)
		break;

	switch (c) {
			case 'n':
				g_daemon = 0;
				break;
			case 's':
				g_syslog = 1;
				break;
			case 'v':
				g_verbose = 1;
				break;
			default:
				print_help();
				exit(EXIT_SUCCESS);
	}
}

if (g_daemon) {
	openlog(__progname, LOG_CONS | LOG_PID, LOG_DAEMON);
	lprintf(LOG_DEBUG, "Daemonizing ...");
	if (access(PID_FILE, F_OK) == 0) {
		lprintf(LOG_ERR, "pid %s exist", PID_FILE);
		closelog();
		exit (EXIT_FAILURE);
	}
	if (-1 == daemon(0, 0)) {
		lprintf(LOG_ERR, "Failed daemonizing: %m");
		closelog();
		exit (EXIT_FAILURE);
	}
	SetPidFile(PID_FILE);
}

lprintf (LOG_INFO, "RPi NAS v %s", VERSION);

if (wiringPiSetup () == -1) {
    lprintf (LOG_ERR, "GPIO Setup failed! %s", strerror (errno));
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
    lprintf (LOG_ERR, "Unable to setup ISR: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (SELECT, INT_EDGE_FALLING, &resetInterrupt) < 0)
  {
    lprintf (LOG_ERR, "Unable to setup ISR: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (LEFT, INT_EDGE_FALLING, &leftInterrupt) < 0)
  {
    lprintf (LOG_ERR, "Unable to setup ISR: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (RIGHT, INT_EDGE_FALLING, &rightInterrupt) < 0)
  {
    lprintf (LOG_ERR, "Unable to setup ISR: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (UP, INT_EDGE_FALLING, &upInterrupt) < 0)
  {
    lprintf (LOG_ERR, "Unable to setup ISR: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }
if (wiringPiISR (DOWN, INT_EDGE_FALLING, &downInterrupt) < 0)
  {
    lprintf (LOG_ERR, "Unable to setup ISR: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }

// configure LCD pins
fd = lcdInit (2,16,4, RS,EN, DB4,DB5,DB6,DB7,0,0,0,0) ;
if (fd == -1)
  {
    lprintf (LOG_ERR, "lcdInit 1 failed: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }

if (softPwmCreate (DISP_LED, 0, DISP_RANGE) < 0)
  {
    lprintf (LOG_ERR, "Unable to setup PWM: %s", strerror (errno)) ;
    exit (EXIT_FAILURE);
  }

lcdClear (fd);
lcdPosition (fd, 0, 0) ;
lcdPrintf (fd, " RPi NAS v%s", VERSION) ;
lcdPosition (fd, 0, 1) ;
lcdPrintf (fd, " was loaded ...") ;
ledOn(0, DISP_RANGE);
delay (1000) ;
lcdClear (fd);
lcdPosition (fd, 0, 0) ;

run = 1;
disp_r = DISP_RANGE;
gettimeofday(&t_last_eth, NULL);
t_last_wlan=t_last_eth;

tim = time (NULL);
ttim = tim + DISP_FLASH_TIME;
buttonRes = -1;
g_quit = 0;

while (!g_quit)
{
    gettimeofday(&t_now, NULL);
    tim = time (NULL) ;
    if (buttonRes != -1 && disp_flag)
	{
	ttim = tim + DISP_FLASH_TIME;
	ledOn(0, disp_r);
	disp_flag = 0;
	}
    if ((tim > ttim) && !disp_flag && run)
	{
	ledOff(0, disp_r);
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
        i--;
	c = 0;
    break;
    case RIGHT:
        buttonRes = -1;
        i++;
	c = 0;
    break;
    case SELECT:
	f=selfunc();
	ledOn(disp_r, disp_r);
	switch (f) {
	    case 0: // SYSTEM
		rpi_shutdown ();
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
	if (retval == EINVAL||EINTR) {
//	    case block
	    switch (i) {
	        case 0:
		    if (c <= DTIME) {
			disp_time() ;
		    } else {
			i++;
			c=0;
		    }
	        break;
	        case 1:
		    if (c <= ETHTIME) {
			ifinfo = IfInfo(ETHx);
			if(ifinfo->ip == NULL){
			    i++;
			    c=0;
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
			LCDprintIfInfo ("E", ifinfo->ip, rx_speed, tx_speed);
			rx_old_eth = bin;
			tx_old_eth = bout;
			t_last_eth = t_now;
			free (ifinfo->ip);
			free (ifinfo);
			} else {
			    i++;
			    c = 0;
			}
	        break;
	        case 2:
		    if (c <= WLANTIME) {
			ifinfo = IfInfo(WLANx);
			if(ifinfo->ip == NULL){
			    i = 0;
			    c = 0;
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
			LCDprintIfInfo ("W", ifinfo->ip, rx_speed, tx_speed);
			rx_old_wlan = bin;
			tx_old_wlan = bout;
			t_last_wlan = t_now;
			free (ifinfo->ip);
			free (ifinfo);
			} else {
			    i = 0;
			    c = 0;
			}
	        break;
	        default:   // MAX_INST
		    disp_time();
		    i = 0;
		    c = 0;
	    }
	    retval = blink();
	    if (run) c++;
      }
      break;
    }
 }
return (EXIT_SUCCESS) ;
}
