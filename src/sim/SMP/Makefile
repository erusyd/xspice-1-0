
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= DCdiveq.c\
		SMPaddElt.c\
		SMPcClear.c\
		SMPcLUfac.c\
		SMPcReorder.c\
		SMPcSolve.c\
		SMPclear.c\
		SMPcolSwap.c\
		SMPcProdDiag.c\
		SMPdestroy.c\
		SMPfillup.c\
		SMPfindElt.c\
		SMPgetError.c\
		SMPluFac.c\
		SMPmakeElt.c\
		SMPmatSize.c\
		SMPnewMatrix.c\
		SMPnewNode.c\
		SMPpreOrder.c\
		SMPprint.c\
		SMPreorder.c\
		SMProwSwap.c\
		SMPsolve.c

COBJS	= DCdiveq.o\
		SMPaddElt.o\
		SMPcClear.o\
		SMPcLUfac.o\
		SMPcReorder.o\
		SMPcSolve.o\
		SMPclear.o\
		SMPcolSwap.o\
		SMPcProdDiag.o\
		SMPdestroy.o\
		SMPfillup.o\
		SMPfindElt.o\
		SMPgetError.o\
		SMPluFac.o\
		SMPmakeElt.o\
		SMPmatSize.o\
		SMPnewMatrix.o\
		SMPnewNode.o\
		SMPpreOrder.o\
		SMPprint.o\
		SMPreorder.o\
		SMProwSwap.o\
		SMPsolve.o


TARGET  = ../SMP

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


