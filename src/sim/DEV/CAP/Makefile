
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		CAP.c\
		CAPacLoad.c\
		CAPask.c\
		CAPdelete.c\
		CAPdestroy.c\
		CAPgetic.c\
		CAPload.c\
		CAPmAsk.c\
		CAPmDelete.c\
		CAPmParam.c\
		CAPparam.c\
		CAPpzLoad.c\
		CAPsAcLoad.c\
		CAPsLoad.c\
		CAPsPrint.c\
		CAPsSetup.c\
		CAPsUpdate.c\
		CAPsetup.c\
		CAPtemp.c\
		CAPtrunc.c

COBJS	= \
		CAP.o\
		CAPacLoad.o\
		CAPask.o\
		CAPdelete.o\
		CAPdestroy.o\
		CAPgetic.o\
		CAPload.o\
		CAPmAsk.o\
		CAPmDelete.o\
		CAPmParam.o\
		CAPparam.o\
		CAPpzLoad.o\
		CAPsAcLoad.o\
		CAPsLoad.o\
		CAPsPrint.o\
		CAPsSetup.o\
		CAPsUpdate.o\
		CAPsetup.o\
		CAPtemp.o\
		CAPtrunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


