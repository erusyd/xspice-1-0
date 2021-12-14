
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		CSW.c\
		CSWask.c\
		CSWacLoad.c\
		CSWdelete.c\
		CSWdestroy.c\
		CSWload.c\
		CSWmAsk.c\
		CSWmDelete.c\
		CSWmParam.c\
		CSWparam.c\
		CSWpzLoad.c\
		CSWsetup.c

COBJS	= \
		CSW.o\
		CSWask.o\
		CSWacLoad.o\
		CSWdelete.o\
		CSWdestroy.o\
		CSWload.o\
		CSWmAsk.o\
		CSWmDelete.o\
		CSWmParam.o\
		CSWparam.o\
		CSWpzLoad.o\
		CSWsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


