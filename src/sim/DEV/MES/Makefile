
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		MES.c\
		MESacLoad.c\
		MESask.c\
		MESdelete.c\
		MESdestroy.c\
		MESgetic.c\
		MESload.c\
		MESmAsk.c\
		MESmDelete.c\
		MESmParam.c\
		MESparam.c\
		MESpzLoad.c\
		MESsetup.c\
		MEStemp.c\
		MEStrunc.c

COBJS	= \
		MES.o\
		MESacLoad.o\
		MESask.o\
		MESdelete.o\
		MESdestroy.o\
		MESgetic.o\
		MESload.o\
		MESmAsk.o\
		MESmDelete.o\
		MESmParam.o\
		MESparam.o\
		MESpzLoad.o\
		MESsetup.o\
		MEStemp.o\
		MEStrunc.o

TARGET  = ../../DEV

INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


