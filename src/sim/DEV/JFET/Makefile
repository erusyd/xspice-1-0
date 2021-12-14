
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		JFET.c\
		JFETask.c\
		JFETacLoad.c\
		JFETdelete.c\
		JFETdestroy.c\
		JFETgetic.c\
		JFETload.c\
		JFETmAsk.c\
		JFETmDelete.c\
		JFETmParam.c\
		JFETparam.c\
		JFETpzLoad.c\
		JFETsetup.c\
		JFETtemp.c\
		JFETtrunc.c

COBJS	= \
		JFET.o\
		JFETask.o\
		JFETacLoad.o\
		JFETdelete.o\
		JFETdestroy.o\
		JFETgetic.o\
		JFETload.o\
		JFETmAsk.o\
		JFETmDelete.o\
		JFETmParam.o\
		JFETparam.o\
		JFETpzLoad.o\
		JFETsetup.o\
		JFETtemp.o\
		JFETtrunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


