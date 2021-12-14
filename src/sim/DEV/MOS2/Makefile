
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		MOS2.c\
		MOS2acLoad.c\
		MOS2ask.c\
		MOS2convTest.c\
		MOS2delete.c\
		MOS2destroy.c\
		MOS2getic.c\
		MOS2load.c\
		MOS2mDelete.c\
		MOS2mParam.c\
		MOS2param.c\
		MOS2pzLoad.c\
		MOS2sAcLoad.c\
		MOS2sLoad.c\
		MOS2sPrint.c\
		MOS2sSetup.c\
		MOS2sUpdate.c\
		MOS2setup.c\
		MOS2temp.c\
		MOS2trunc.c

COBJS	= \
		MOS2.o\
		MOS2acLoad.o\
		MOS2ask.o\
		MOS2convTest.o\
		MOS2delete.o\
		MOS2destroy.o\
		MOS2getic.o\
		MOS2load.o\
		MOS2mDelete.o\
		MOS2mParam.o\
		MOS2param.o\
		MOS2pzLoad.o\
		MOS2sAcLoad.o\
		MOS2sLoad.o\
		MOS2sPrint.o\
		MOS2sSetup.o\
		MOS2sUpdate.o\
		MOS2setup.o\
		MOS2temp.o\
		MOS2trunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


