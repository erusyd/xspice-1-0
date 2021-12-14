
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		SW.c\
		SWacLoad.c\
		SWask.c\
		SWdelete.c\
		SWdestroy.c\
		SWload.c\
		SWmAsk.c\
		SWmDelete.c\
		SWmParam.c\
		SWparam.c\
		SWpzLoad.c\
		SWsetup.c

COBJS	= \
		SW.o\
		SWacLoad.o\
		SWask.o\
		SWdelete.o\
		SWdestroy.o\
		SWload.o\
		SWmAsk.o\
		SWmDelete.o\
		SWmParam.o\
		SWparam.o\
		SWpzLoad.o\
		SWsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


