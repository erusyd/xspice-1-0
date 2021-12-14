
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		URC.c\
		URCask.c\
		URCdelete.c\
		URCdestroy.c\
		URCmAsk.c\
		URCmDelete.c\
		URCmParam.c\
		URCparam.c\
		URCsetup.c

COBJS	= \
		URC.o\
		URCask.o\
		URCdelete.o\
		URCdestroy.o\
		URCmAsk.o\
		URCmDelete.o\
		URCmParam.o\
		URCparam.o\
		URCsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


