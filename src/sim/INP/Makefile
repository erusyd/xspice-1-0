
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		IFeval.c\
		IFnewUid.c\
		INP2B.c\
		INP2C.c\
		INP2D.c\
		INP2E.c\
		INP2F.c\
		INP2G.c\
		INP2H.c\
		INP2I.c\
		INP2J.c\
		INP2K.c\
		INP2L.c\
		INP2M.c\
		INP2Q.c\
		INP2R.c\
		INP2S.c\
		INP2T.c\
		INP2U.c\
		INP2V.c\
		INP2W.c\
		INP2Z.c\
		INP2dot.c\
		INPaName.c\
		INPapName.c\
		INPcaseFix.c\
		INPdevParse.c\
		INPdoOpts.c\
		INPdomodel.c\
		INPerrCat.c\
		INPerror.c\
		INPevaluate.c\
		INPfindLev.c\
		INPgetMod.c\
		INPgetTitle.c\
		INPgetTok.c\
		INPgetValue.c\
		INPkillMods.c\
		INPlist.c\
		INPlookMod.c\
		INPmakeMod.c\
		INPmkTemp.c\
		INPpName.c\
		INPparseTree.c\
		INPpas1.c\
		INPpas2.c\
		INPsymTab.c\
		INPtypelook.c\
		PTfunctions.c

COBJS	= \
		IFeval.o\
		IFnewUid.o\
		INP2B.o\
		INP2C.o\
		INP2D.o\
		INP2E.o\
		INP2F.o\
		INP2G.o\
		INP2H.o\
		INP2I.o\
		INP2J.o\
		INP2K.o\
		INP2L.o\
		INP2M.o\
		INP2Q.o\
		INP2R.o\
		INP2S.o\
		INP2T.o\
		INP2U.o\
		INP2V.o\
		INP2W.o\
		INP2Z.o\
		INP2dot.o\
		INPaName.o\
		INPapName.o\
		INPcaseFix.o\
		INPdevParse.o\
		INPdoOpts.o\
		INPdomodel.o\
		INPerrCat.o\
		INPerror.o\
		INPevaluate.o\
		INPfindLev.o\
		INPgetMod.o\
		INPgetTitle.o\
		INPgetTok.o\
		INPgetValue.o\
		INPkillMods.o\
		INPlist.o\
		INPlookMod.o\
		INPmakeMod.o\
		INPmkTemp.o\
		INPpName.o\
		INPparseTree.o\
		INPpas1.o\
		INPpas2.o\
		INPsymTab.o\
		INPtypelook.o\
		PTfunctions.o

TARGET  = ../INP

#----------------------------------------------------------------------------

INCLUDE=-I../include

PG = 

CFLAGS = 

LDFLAGS = -z -lm

.c.o: $*.c
	${CC} ${CFLAGS} ${PG} ${INCLUDE} -c $*.c

all: library Proc2Mod

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a

Proc2Mod: Proc2Mod.o library
	${CC} ${CFLAGS} -o Proc2Mod Proc2Mod.o ${TARGET}.a ${LDFLAGS}

Proc2Mod.o: Proc2Mod.c 
	${CC} ${CFLAGS} ${PG} ${INCLUDE} -c Proc2Mod.c


