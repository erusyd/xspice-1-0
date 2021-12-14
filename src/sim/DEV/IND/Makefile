
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		IND.c\
		INDacLoad.c\
		INDask.c\
		INDdelete.c\
		INDdestroy.c\
		INDload.c\
		INDmDelete.c\
		INDparam.c\
		INDpzLoad.c\
		INDsAcLoad.c\
		INDsLoad.c\
		INDsPrint.c\
		INDsSetup.c\
		INDsUpdate.c\
		INDsetup.c\
		INDtrunc.c\
		MUTacLoad.c\
		MUTask.c\
		MUTdelete.c\
		MUTdestroy.c\
		MUTmDelete.c\
		MUTparam.c\
		MUTpzLoad.c\
		MUTsPrint.c\
		MUTsSetup.c\
		MUTsetup.c

COBJS	= \
		IND.o\
		INDacLoad.o\
		INDask.o\
		INDdelete.o\
		INDdestroy.o\
		INDload.o\
		INDmDelete.o\
		INDparam.o\
		INDpzLoad.o\
		INDsAcLoad.o\
		INDsLoad.o\
		INDsPrint.o\
		INDsSetup.o\
		INDsUpdate.o\
		INDsetup.o\
		INDtrunc.o\
		MUTacLoad.o\
		MUTask.o\
		MUTdelete.o\
		MUTdestroy.o\
		MUTmDelete.o\
		MUTparam.o\
		MUTpzLoad.o\
		MUTsPrint.o\
		MUTsSetup.o\
		MUTsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


