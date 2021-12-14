
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		ASRC.c\
		ASRCask.c\
		ASRCacLoad.c\
		ASRCconvTest.c\
		ASRCdelete.c\
		ASRCdestroy.c\
		ASRCfindBr.c\
		ASRCload.c\
		ASRCmDelete.c\
		ASRCparam.c\
		ASRCpzLoad.c\
		ASRCsetup.c

COBJS	= \
		ASRC.o\
		ASRCask.o\
		ASRCacLoad.o\
		ASRCconvTest.o\
		ASRCdelete.o\
		ASRCdestroy.o\
		ASRCfindBr.o\
		ASRCload.o\
		ASRCmDelete.o\
		ASRCparam.o\
		ASRCpzLoad.o\
		ASRCsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 


.c.o: $*.c
	${CC} ${CFLAGS} ${PG} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


