
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES =\
	alias.c\
	backquote.c\
	complete.c\
	cshpar.c\
	front.c\
	glob.c\
	history.c\
	lexical.c\
	modify.c\
	output.c\
	quote.c\
	spawn.c\
	std.c\
	unixcom.c\
	variable.c\
	wlist.c

COBJS =\
	alias.o\
	backquote.o\
	complete.o\
	cshpar.o\
	front.o\
	glob.o\
	history.o\
	lexical.o\
	modify.o\
	output.o\
	quote.o\
	spawn.o\
	std.o\
	unixcom.o\
	variable.o\
	wlist.o

HFILES	=\
	../include/FTEcmath.h\
	../include/FTEconstant.h\
	../include/FTEcshpar.h\
	../include/FTEdata.h\
	../include/FTEdebug.h\
	../include/FTEdefs.h\
	../include/FTEinput.h\
	../include/FTEparse.h\
	../include/FTEspicedata.h\
	../include/FTEwritedata.h

TARGET  = ../CP

INCLUDE = -I. -I../include

CFLAGS = 

.c.o: $*.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $*.c

library:  $(TARGET).a input.o

$(TARGET).a: $(COBJS)
	ar r $(TARGET).a $?
	ranlib $(TARGET).a


