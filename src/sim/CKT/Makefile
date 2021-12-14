
# Include global XSPICE selections for CC and other macros
include /usr/local/xspice-1-0/include/make.include


CFILES	= \
		ACan.c\
		ACaskQuest.c\
		ACsetParm.c\
		CKTaccept.c\
		CKTacct.c\
		CKTacDump.c\
		CKTacLoad.c\
		CKTask.c\
		CKTaskAnalQ.c\
		CKTaskNodQst.c\
		CKTbindNode.c\
		CKTbreakDump.c\
		CKTclrBreak.c\
		CKTconvTest.c\
		CKTcrtElt.c\
		CKTdelTask.c\
		CKTdestroy.c\
		CKTdltInst.c\
		CKTdltMod.c\
		CKTdltNod.c\
		CKTdoJob.c\
		CKTdump.c\
		CKTfndAnal.c\
		CKTfndBranch.c\
		CKTfndDev.c\
		CKTfndMod.c\
		CKTfndNode.c\
		CKTfndTask.c\
		CKTground.c\
		CKTinit.c\
		CKTic.c\
		CKTinst2Node.c\
		CKTlinkEq.c\
		CKTload.c\
		CKTmkCur.c\
		CKTmkNode.c\
		CKTmkVolt.c\
		CKTmapNode.c\
		CKTmodAsk.c\
		CKTmodCrt.c\
		CKTmodParam.c\
		CKTnames.c\
		CKTnewAnal.c\
		CKTnewEq.c\
		CKTnewNode.c\
		CKTnewTask.c\
		CKTnodName.c\
		CKTnum2nod.c\
		CKTop.c\
		CKTparam.c\
		CKTpModName.c\
		CKTpName.c\
		CKTpzLoad.c\
		CKTpzSetup.c\
		CKTsenAC.c\
		CKTsenComp.c\
		CKTsenDCtran.c\
		CKTsenLoad.c\
		CKTsenPrint.c\
		CKTsenSetup.c\
		CKTsenUpdate.c\
		CKTsetAnalPm.c\
		CKTsetBreak.c\
		CKTsetNodPm.c\
		CKTsetOpt.c\
		CKTsetup.c\
		CKTtemp.c\
		CKTterr.c\
		CKTtrunc.c\
		CKTtypelook.c\
		DCOaskQuest.c\
		DCOsetParm.c\
		DCTaskQuest.c\
		DCTsetParm.c\
		DCop.c\
		DCtran.c\
		DCtrCurv.c\
		SENstartup.c\
		SENsetParm.c\
		SENaskQuest.c\
		SENdestroy.c\
		SPerror.c\
		PZan.c\
		PZaskQuest.c\
		PZsetParm.c\
		TFanal.c\
		TFsetParm.c\
		TFaskQuest.c\
		TRANaskQuest.c\
		TRANsetParm.c

COBJS	= \
		ACan.o\
		ACaskQuest.o\
		ACsetParm.o\
		CKTaccept.o\
		CKTacct.o\
		CKTacDump.o\
		CKTacLoad.o\
		CKTask.o\
		CKTaskAnalQ.o\
		CKTaskNodQst.o\
		CKTbindNode.o\
		CKTbreakDump.o\
		CKTclrBreak.o\
		CKTconvTest.o\
		CKTcrtElt.o\
		CKTdelTask.o\
		CKTdestroy.o\
		CKTdltInst.o\
		CKTdltMod.o\
		CKTdltNod.o\
		CKTdoJob.o\
		CKTdump.o\
		CKTfndAnal.o\
		CKTfndBranch.o\
		CKTfndDev.o\
		CKTfndMod.o\
		CKTfndNode.o\
		CKTfndTask.o\
		CKTground.o\
		CKTinit.o\
		CKTic.o\
		CKTinst2Node.o\
		CKTlinkEq.o\
		CKTload.o\
		CKTmkCur.o\
		CKTmkNode.o\
		CKTmkVolt.o\
		CKTmapNode.o\
		CKTmodAsk.o\
		CKTmodCrt.o\
		CKTmodParam.o\
		CKTnames.o\
		CKTnewAnal.o\
		CKTnewEq.o\
		CKTnewNode.o\
		CKTnewTask.o\
		CKTnodName.o\
		CKTnum2nod.o\
		CKTop.o\
		CKTparam.o\
		CKTpModName.o\
		CKTpName.o\
		CKTpzLoad.o\
		CKTpzSetup.o\
		CKTsenAC.o\
		CKTsenComp.o\
		CKTsenDCtran.o\
		CKTsenLoad.o\
		CKTsenPrint.o\
		CKTsenSetup.o\
		CKTsenUpdate.o\
		CKTsetAnalPm.o\
		CKTsetBreak.o\
		CKTsetNodPm.o\
		CKTsetOpt.o\
		CKTsetup.o\
		CKTtemp.o\
		CKTterr.o\
		CKTtrunc.o\
		CKTtypelook.o\
		DCOaskQuest.o\
		DCOsetParm.o\
		DCTaskQuest.o\
		DCTsetParm.o\
		DCop.o\
		DCtran.o\
		DCtrCurv.o\
		SENstartup.o\
		SENsetParm.o\
		SENaskQuest.o\
		SENdestroy.o\
		SPerror.o\
		PZan.o\
		PZaskQuest.o\
		PZsetParm.o\
		TFanal.o\
		TFsetParm.o\
		TFaskQuest.o\
		TRANaskQuest.o\
		TRANsetParm.o

TARGET  = ../CKT

#----------------------------------------------------------------------------

INCLUDE=-I../include

CFLAGS = 

.c.o: $*.c
	${CC} ${CFLAGS} ${INCLUDE} -c $*.c

library:  ${TARGET}.a

${TARGET}.a:  ${COBJS}
	ar r ${TARGET}.a $?
	ranlib ${TARGET}.a


