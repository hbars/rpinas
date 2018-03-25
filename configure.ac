dnl Process this file with autoconf to produce a configure script.

AC_INIT([nas], [1.4], [hbars@bk.ru])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(conf.h)

AC_PATH_PROG([IW],[iw])
AC_PATH_PROG([REBOOT],[reboot])
AC_PATH_PROG([POWEROFF],[poweroff])

AC_DEFINE_UNQUOTED([IW], ["$IW"], [wlan config])
AC_DEFINE_UNQUOTED([REBOOT], ["$REBOOT"], [system reboot])
AC_DEFINE_UNQUOTED([POWEROFF], ["$POWEROFF"], [power off])

AC_CHECK_LIB(wiringPi, wiringPiISR)
AC_CHECK_LIB(wiringPiDev, lcdInit)
AC_HAVE_HEADERS([wiringPi.h lcd.h softPwm.h])
AC_HAVE_HEADERS([time.h])
AC_HEADER_TIME

AC_OUTPUT([Makefile])
AC_MSG_RESULT([])
AC_MSG_RESULT([Type 'make'])
AC_MSG_RESULT([])
