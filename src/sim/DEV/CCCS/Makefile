
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		CCCS.c\
		CCCSask.c\
		CCCSdelete.c\
		CCCSdestroy.c\
		CCCSload.c\
		CCCSmDelete.c\
		CCCSparam.c\
		CCCSpzLoad.c\
		CCCSsAcLoad.c\
		CCCSsLoad.c\
		CCCSsPrint.c\
		CCCSsSetup.c\
		CCCSsetup.c

COBJS	= \
		CCCS.o\
		CCCSask.o\
		CCCSdelete.o\
		CCCSdestroy.o\
		CCCSload.o\
		CCCSmDelete.o\
		CCCSparam.o\
		CCCSpzLoad.o\
		CCCSsAcLoad.o\
		CCCSsLoad.o\
		CCCSsPrint.o\
		CCCSsSetup.o\
		CCCSsetup.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


