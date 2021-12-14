
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		ISRC.c\
		ISRCask.c\
		ISRCaccept.c\
		ISRCacLoad.c\
		ISRCdelete.c\
		ISRCdestroy.c\
		ISRCload.c\
		ISRCmDelete.c\
		ISRCparam.c\
		ISRCtemp.c

COBJS	= \
		ISRC.o\
		ISRCask.o\
		ISRCaccept.o\
		ISRCacLoad.o\
		ISRCdelete.o\
		ISRCdestroy.o\
		ISRCload.o\
		ISRCmDelete.o\
		ISRCparam.o\
		ISRCtemp.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


