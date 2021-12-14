
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
MIF_INP2A.c \
MIFgetMod.c \
MIFgetValue.c \
MIFload.c \
MIFmParam.c \
MIFsetup.c \
MIFutil.c \
MIFask.c \
MIFmAsk.c \
MIFtrunc.c \
MIFconvTest.c \
MIFdelete.c \
MIFmDelete.c \
MIFdestroy.c \
MIF.c

COBJS	= \
MIF_INP2A.o \
MIFgetMod.o \
MIFgetValue.o \
MIFload.o \
MIFmParam.o \
MIFsetup.o \
MIFutil.o \
MIFask.o \
MIFmAsk.o \
MIFtrunc.o \
MIFconvTest.o \
MIFdelete.o \
MIFmDelete.o \
MIFdestroy.o \
MIF.o

TARGET  = ../MIF

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS =

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c


library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


