
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
ENHtranslate_poly.c \
ENH.c

COBJS	= \
ENHtranslate_poly.o \
ENH.o

TARGET  = ../ENH

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


