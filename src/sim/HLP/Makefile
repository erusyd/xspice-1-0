
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES =\
	help.c\
	provide.c\
	readhelp.c\
	textdisplay.c\
	xdisplay.c \
	x11display.c

COBJS =\
	help.o\
	provide.o\
	readhelp.o\
	textdisplay.o\
	xdisplay.o \
	x11display.o

HFILES	=\
	../include/HLPdefs.h\
	../include/CPstd.h

TARGET  = ../HLP

INCLUDE = -I../include -I/usr/include/X11/Xaw -I/usr/include/X11

CFLAGS = 

LIBX = -L/usr/X11/lib -lXaw -lXt -lXmu -lX11


.c.o: $*.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $*.c



all: library help makeindex



library:  $(TARGET).a

$(TARGET).a: $(COBJS)
	ar r $(TARGET).a $?
	ranlib $(TARGET).a



help: helpmain.o library
	$(CC) $(CFLAGS) ${INCLUDE} -o help helpmain.o \
		$(TARGET).a ../CP.a ${LIBX} -ltermlib

makeindex: makeindex.o
	${CC} ${CFLAGS} ${INCLUDES} -o makeindex makeindex.o

makeindex.o: makeindex.c


