
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		VCCS.c\
		VCCSask.c\
		VCCSdelete.c\
		VCCSdestroy.c\
		VCCSload.c\
		VCCSmDelete.c\
		VCCSparam.c\
		VCCSpzLoad.c\
		VCCSsAcLoad.c\
		VCCSsLoad.c\
		VCCSsPrint.c\
		VCCSsSetup.c\
		VCCSsetup.c

COBJS	= \
		VCCS.o\
		VCCSask.o\
		VCCSdelete.o\
		VCCSdestroy.o\
		VCCSload.o\
		VCCSmDelete.o\
		VCCSparam.o\
		VCCSpzLoad.o\
		VCCSsAcLoad.o\
		VCCSsLoad.o\
		VCCSsPrint.o\
		VCCSsSetup.o\
		VCCSsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


