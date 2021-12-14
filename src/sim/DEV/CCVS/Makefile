
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		CCVS.c\
		CCVSask.c\
		CCVSdelete.c\
		CCVSdestroy.c\
		CCVSfindBr.c\
		CCVSload.c\
		CCVSmDelete.c\
		CCVSparam.c\
		CCVSpzLoad.c\
		CCVSsAcLoad.c\
		CCVSsLoad.c\
		CCVSsPrint.c\
		CCVSsSetup.c\
		CCVSsetup.c

COBJS	= \
		CCVS.o\
		CCVSask.o\
		CCVSdelete.o\
		CCVSdestroy.o\
		CCVSfindBr.o\
		CCVSload.o\
		CCVSmDelete.o\
		CCVSparam.o\
		CCVSpzLoad.o\
		CCVSsAcLoad.o\
		CCVSsLoad.o\
		CCVSsPrint.o\
		CCVSsSetup.o\
		CCVSsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE}  -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


