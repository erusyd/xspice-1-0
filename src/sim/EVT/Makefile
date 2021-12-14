
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
EVTaccept.c        \
EVTbackup.c        \
EVTcall_hybrids.c  \
EVTdequeue.c       \
EVTinit.c         \
EVTiter.c          \
EVTload.c          \
EVTnext_time.c     \
EVTnode_copy.c     \
EVTop.c            \
EVTprint.c         \
EVTplot.c         \
EVTqueue.c         \
EVTsetup.c         \
EVTdump.c         \
EVTtermInsert.c

COBJS	= \
EVTaccept.o        \
EVTbackup.o        \
EVTcall_hybrids.o  \
EVTdequeue.o       \
EVTinit.o         \
EVTiter.o          \
EVTload.o          \
EVTnext_time.o     \
EVTnode_copy.o     \
EVTop.o            \
EVTprint.o         \
EVTplot.o         \
EVTqueue.o         \
EVTsetup.o         \
EVTdump.o         \
EVTtermInsert.o

TARGET  = ../EVT

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


