dnl Process this file with autoconf to produce a configure script.

AC_INIT([nas], [2.0], [hbars@bk.ru])
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([nas.c])
AC_CONFIG_HEADER([conf.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_INSTALL

AC_HEADER_STDC
AC_CHECK_HEADERS(stdio.h stdlib.h stdint.h string.h unistd.h errno.h)
AC_CHECK_HEADERS(syslog.h signal.h getopt.h)
AC_CHECK_HEADERS(ifaddrs.h netdb.h linux/if_link.h arpa/inet.h sys/socket.h)
AC_CHECK_HEADERS(regex.h sys/types.h locale.h ctype.h fcntl.h)
AC_HAVE_HEADERS([wiringPi.h lcd.h softPwm.h])
AC_HAVE_HEADERS([time.h])
AC_HEADER_TIME
AC_CHECK_LIB(wiringPi, wiringPiISR)
AC_CHECK_LIB(wiringPiDev, lcdInit)

AC_PATH_PROG([IW],[iw])
AC_PATH_PROG([REBOOT],[reboot])
AC_PATH_PROG([POWEROFF],[poweroff])
AC_DEFINE_UNQUOTED([IW], ["$IW"], [wlan config])
AC_DEFINE_UNQUOTED([REBOOT], ["$REBOOT"], [system reboot])
AC_DEFINE_UNQUOTED([POWEROFF], ["$POWEROFF"], [power off])

AC_OUTPUT
AC_MSG_RESULT([])
AC_MSG_RESULT([Type 'make'])
AC_MSG_RESULT([])
