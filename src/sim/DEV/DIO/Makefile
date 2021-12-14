
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		DIO.c\
		DIOask.c\
		DIOacLoad.c\
		DIOconvTest.c\
		DIOdelete.c\
		DIOdestroy.c\
		DIOgetic.c\
		DIOload.c\
		DIOmAsk.c\
		DIOmDelete.c\
		DIOmParam.c\
		DIOparam.c\
		DIOpzLoad.c\
		DIOsAcLoad.c\
		DIOsLoad.c\
		DIOsPrint.c\
		DIOsSetup.c\
		DIOsUpdate.c\
		DIOsetup.c\
		DIOtemp.c\
		DIOtrunc.c

COBJS	= \
		DIO.o\
		DIOask.o\
		DIOacLoad.o\
		DIOconvTest.o\
		DIOdelete.o\
		DIOdestroy.o\
		DIOgetic.o\
		DIOload.o\
		DIOmAsk.o\
		DIOmDelete.o\
		DIOmParam.o\
		DIOparam.o\
		DIOpzLoad.o\
		DIOsAcLoad.o\
		DIOsLoad.o\
		DIOsPrint.o\
		DIOsSetup.o\
		DIOsUpdate.o\
		DIOsetup.o\
		DIOtemp.o\
		DIOtrunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


