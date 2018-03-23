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
#include "poweroff.h"

void rpi_shutdown () {
int ret1,ret2;

ret1 = pwch1 ();
ret2 = pwch2 ();
if (ret2 == LEFT) {
    lcdClear (fd);
    return;
}

if (ret1 == UP)
    {
// reboot
    byebye ();
    system (REBOOT);
    exit (EXIT_SUCCESS);
    }
else
    {
// poweroff
    byebye ();
    system (POWEROFF);
    exit (EXIT_SUCCESS);
    }

return;
}

int pwch1 () {

    lcdClear (fd);
    lcdPosition (fd, 1, 0) ;
    lcdPrintf (fd, "REBOOT  :") ;
    lcdPosition (fd, 1, 1) ;
    lcdPrintf (fd, "POWEROFF:") ;

    return UPorDOWN (12);
}

int pwch2 () {

    lcdClear (fd);
    lcdPosition (fd, 1, 0) ;
    lcdPrintf (fd, "A YOU SHURE: N") ;

    return YorN (14,0);
}

void byebye () {

    lcdClear (fd);
    lcdPosition (fd, 1, 0) ;
    lcdPrintf (fd, " BYE, BYE...") ;
    delay (250) ;
    lcdClear (fd);
    ledOff (0, DISP_RANGE);
}
