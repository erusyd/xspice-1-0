
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		BSIM.c\
		BSIMacLoad.c\
		BSIMask.c\
		BSIMconvTest.c\
		BSIMdelete.c\
		BSIMdestroy.c\
		BSIMevaluate.c\
		BSIMgetic.c\
		BSIMload.c\
		BSIMmAsk.c\
		BSIMmDelete.c\
		BSIMmParam.c\
		BSIMmosCap.c\
		BSIMparam.c\
		BSIMpzLoad.c\
		BSIMsetup.c\
		BSIMtemp.c\
		BSIMtrunc.c

COBJS	= \
		BSIM.o\
		BSIMacLoad.o\
		BSIMask.o\
		BSIMconvTest.o\
		BSIMdelete.o\
		BSIMdestroy.o\
		BSIMevaluate.o\
		BSIMgetic.o\
		BSIMload.o\
		BSIMmAsk.o\
		BSIMmDelete.o\
		BSIMmParam.o\
		BSIMmosCap.o\
		BSIMparam.o\
		BSIMpzLoad.o\
		BSIMsetup.o\
		BSIMtemp.o\
		BSIMtrunc.o

TARGET = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${PG} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


