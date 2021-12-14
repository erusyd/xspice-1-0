
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
IDNdig.c 

COBJS	= \
IDNdig.o 

TARGET  = ../IDN

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


