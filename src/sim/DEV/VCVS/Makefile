
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		VCVS.c\
		VCVSask.c\
		VCVSdelete.c\
		VCVSdestroy.c\
		VCVSfindBr.c\
		VCVSload.c\
		VCVSmDelete.c\
		VCVSparam.c\
		VCVSpzLoad.c\
		VCVSsAcLoad.c\
		VCVSsLoad.c\
		VCVSsPrint.c\
		VCVSsSetup.c\
		VCVSsetup.c

COBJS	= \
		VCVS.o\
		VCVSask.o\
		VCVSdelete.o\
		VCVSdestroy.o\
		VCVSfindBr.o\
		VCVSload.o\
		VCVSmDelete.o\
		VCVSparam.o\
		VCVSpzLoad.o\
		VCVSsAcLoad.o\
		VCVSsLoad.o\
		VCVSsPrint.o\
		VCVSsSetup.o\
		VCVSsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


