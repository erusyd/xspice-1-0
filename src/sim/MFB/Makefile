
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	=\
	mfb3d.c\
	mfbbasic.c\
	mfbcaps.c\
	mfbclip.c\
	mfbdecode.c\
	mfbgncode.c\
	mfbio.c\
	mfbtext.c\
	paths.c

COBJS	=\
	mfb3d.o\
	mfbbasic.o\
	mfbcaps.o\
	mfbclip.o\
	mfbdecode.o\
	mfbgncode.o\
	mfbio.o\
	mfbtext.o\
	paths.o

HFILES	=\
	mfb.h

VMSFILES = vmsio.c

TARGET  = ../MFB

INCLUDE= -I. -I../include

CFLAGS = 

.c.o: $*.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $*.c


all: library ${VMSFILES}

library:  $(TARGET).a

$(TARGET).a:  $(COBJS)
	ar r $(TARGET).a $?
	ranlib $(TARGET).a


