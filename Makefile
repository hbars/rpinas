#
# Makefile:
#
#	Copyright (c) 2016 Tolya Borisenkov
#################################################################################
# This file is part of RPi NAS
################################################################################


#DEBUG	= -g -O0
DEBUG	= -O2
CC	= gcc
INCLUDE	+= -I/usr/include
CFLAGS	+= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/lib
#LDLIBS	= -lwiringPi -lwiringPiDev -lm
LDLIBS	= -lwiringPi -lwiringPiDev

# Should not alter anything below this line
###############################################################################

SRC	=	nas.c poweroff.c func.c

OBJ	=	$(SRC:.c=.o)

#BINS	=	$(SRC:.c=)
BINS	=	nas

nas:	$(OBJ)
	@echo [link]
	@$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

depend:
	makedepend -Y $(SRC)

install:
	@echo Nothing to be made.

# DO NOT DELETE

