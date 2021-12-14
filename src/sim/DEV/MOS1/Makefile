
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		MOS1.c\
		MOS1acLoad.c\
		MOS1ask.c\
		MOS1convTest.c\
		MOS1delete.c\
		MOS1destroy.c\
		MOS1getic.c\
		MOS1load.c\
		MOS1mAsk.c\
		MOS1mDelete.c\
		MOS1mParam.c\
		MOS1param.c\
		MOS1pzLoad.c\
		MOS1sAcLoad.c\
		MOS1sLoad.c\
		MOS1sPrint.c\
		MOS1sSetup.c\
		MOS1sUpdate.c\
		MOS1setup.c\
		MOS1temp.c\
		MOS1trunc.c

COBJS	= \
		MOS1.o\
		MOS1acLoad.o\
		MOS1ask.o\
		MOS1convTest.o\
		MOS1delete.o\
		MOS1destroy.o\
		MOS1getic.o\
		MOS1load.o\
		MOS1mAsk.o\
		MOS1mDelete.o\
		MOS1mParam.o\
		MOS1param.o\
		MOS1pzLoad.o\
		MOS1sAcLoad.o\
		MOS1sLoad.o\
		MOS1sPrint.o\
		MOS1sSetup.o\
		MOS1sUpdate.o\
		MOS1setup.o\
		MOS1temp.o\
		MOS1trunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


