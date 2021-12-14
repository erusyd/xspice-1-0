
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		TRA.c\
		TRAacLoad.c\
		TRAaccept.c\
		TRAask.c\
		TRAdelete.c\
		TRAdestroy.c\
		TRAload.c\
		TRAmDelete.c\
		TRAparam.c\
		TRAsetup.c\
		TRAtemp.c\
		TRAtrunc.c

COBJS	= \
		TRA.o\
		TRAacLoad.o\
		TRAaccept.o\
		TRAask.o\
		TRAdelete.o\
		TRAdestroy.o\
		TRAload.o\
		TRAmDelete.o\
		TRAparam.o\
		TRAsetup.o\
		TRAtemp.o\
		TRAtrunc.o

TARGET  = ../../DEV


INCLUDE=-I../../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a
${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


