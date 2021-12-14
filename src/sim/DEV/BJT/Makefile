
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		BJT.c\
		BJTacLoad.c\
		BJTask.c\
		BJTconvTest.c\
		BJTdelete.c\
		BJTdestroy.c\
		BJTgetic.c\
		BJTload.c\
		BJTmAsk.c\
		BJTmDelete.c\
		BJTmParam.c\
		BJTparam.c\
		BJTpzLoad.c\
		BJTsAcLoad.c\
		BJTsLoad.c\
		BJTsPrint.c\
		BJTsSetup.c\
		BJTsUpdate.c\
		BJTsetup.c\
		BJTtemp.c\
		BJTtrunc.c

COBJS	= \
		BJT.o\
		BJTacLoad.o\
		BJTask.o\
		BJTconvTest.o\
		BJTdelete.o\
		BJTdestroy.o\
		BJTgetic.o\
		BJTload.o\
		BJTmAsk.o\
		BJTmDelete.o\
		BJTmParam.o\
		BJTparam.o\
		BJTpzLoad.o\
		BJTsAcLoad.o\
		BJTsLoad.o\
		BJTsPrint.o\
		BJTsSetup.o\
		BJTsUpdate.o\
		BJTsetup.o\
		BJTtemp.o\
		BJTtrunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${PG} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


