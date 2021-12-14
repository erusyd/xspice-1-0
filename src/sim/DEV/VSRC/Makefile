
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		VSRC.c\
		VSRCacLoad.c\
		VSRCaccept.c\
		VSRCask.c\
		VSRCdelete.c\
		VSRCdestroy.c\
		VSRCfindBr.c\
		VSRCload.c\
		VSRCmDelete.c\
		VSRCparam.c\
		VSRCpzLoad.c\
		VSRCpzSetup.c\
		VSRCsetup.c\
		VSRCtemp.c

COBJS	= \
		VSRC.o\
		VSRCacLoad.o\
		VSRCaccept.o\
		VSRCask.o\
		VSRCdelete.o\
		VSRCdestroy.o\
		VSRCfindBr.o\
		VSRCload.o\
		VSRCmDelete.o\
		VSRCparam.o\
		VSRCpzLoad.o\
		VSRCpzSetup.o\
		VSRCsetup.o\
		VSRCtemp.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


