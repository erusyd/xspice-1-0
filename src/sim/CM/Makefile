
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include

CFILES	= \
CM.c \
CMevt.c \
CMutil.c \
CMmeters.c

COBJS	= \
CM.o \
CMevt.o \
CMutil.o \
CMmeters.o

TARGET  = ../CM

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


