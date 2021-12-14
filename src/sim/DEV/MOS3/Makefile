
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		MOS3.c\
		MOS3acLoad.c\
		MOS3ask.c\
		MOS3convTest.c\
		MOS3delete.c\
		MOS3destroy.c\
		MOS3getic.c\
		MOS3load.c\
		MOS3mAsk.c\
		MOS3mDelete.c\
		MOS3mParam.c\
		MOS3param.c\
		MOS3pzLoad.c\
		MOS3sAcLoad.c\
		MOS3sLoad.c\
		MOS3sPrint.c\
		MOS3sSetup.c\
		MOS3sUpdate.c\
		MOS3setup.c\
		MOS3temp.c\
		MOS3trunc.c

COBJS	= \
		MOS3.o\
		MOS3acLoad.o\
		MOS3ask.o\
		MOS3convTest.o\
		MOS3delete.o\
		MOS3destroy.o\
		MOS3getic.o\
		MOS3load.o\
		MOS3mAsk.o\
		MOS3mDelete.o\
		MOS3mParam.o\
		MOS3param.o\
		MOS3pzLoad.o\
		MOS3sAcLoad.o\
		MOS3sLoad.o\
		MOS3sPrint.o\
		MOS3sSetup.o\
		MOS3sUpdate.o\
		MOS3setup.o\
		MOS3temp.o\
		MOS3trunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


