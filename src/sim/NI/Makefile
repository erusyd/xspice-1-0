
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		NIacIter.c\
		NIcomCof.c\
		NIconvTest.c\
		NIdestroy.c\
		NIinit.c\
		NIintegrate.c\
		NIiter.c\
		NIpred.c\
		NIpzMuller.c\
		NIpzSolve.c\
		NIreinit.c\
		NIsenReinit.c

COBJS	= \
		NIacIter.o\
		NIcomCof.o\
		NIconvTest.o\
		NIdestroy.o\
		NIinit.o\
		NIintegrate.o\
		NIiter.o\
		NIpred.o\
		NIpzMuller.o\
		NIpzSolve.o\
		NIreinit.o\
		NIsenReinit.o

TARGET  = ../NI

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${PG} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


