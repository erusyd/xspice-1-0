
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		RES.c\
		RESask.c\
		RESdelete.c\
		RESdestroy.c\
		RESload.c\
		RESmAsk.c\
		RESmDelete.c\
		RESmParam.c\
		RESparam.c\
		RESpzLoad.c\
		RESsAcLoad.c\
		RESsLoad.c\
		RESsSetup.c\
		RESsPrint.c\
		RESsetup.c\
		REStemp.c

COBJS	= \
		RES.o\
		RESask.o\
		RESdelete.o\
		RESdestroy.o\
		RESload.o\
		RESmAsk.o\
		RESmDelete.o\
		RESmParam.o\
		RESparam.o\
		RESpzLoad.o\
		RESsAcLoad.o\
		RESsLoad.o\
		RESsSetup.o\
		RESsPrint.o\
		RESsetup.o\
		REStemp.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


