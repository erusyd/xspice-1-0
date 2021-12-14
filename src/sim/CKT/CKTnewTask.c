/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "TSKdefs.h"
#include "IFsim.h"
#include "util.h"
#include "CKTdefs.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "suffix.h"

RCSID("CKTnewTask.c $Revision: 1.2 $ on $Date: 91/04/17 17:15:57 $")

/* ARGSUSED */
int
CKTnewTask(ckt,taskPtr,taskName)
    GENERIC *ckt;
    GENERIC **taskPtr;
    IFuid taskName;
{
    register TSKtask *tsk;
    *taskPtr = (GENERIC *)MALLOC(sizeof(TSKtask));
    if(*taskPtr==NULL) return(E_NOMEM);
    tsk = *(TSKtask **)taskPtr;
    tsk->TSKname = taskName;
    tsk->TSKgmin = 1e-12;
    tsk->TSKabstol = 1e-12;
    tsk->TSKreltol = 1e-3;
    tsk->TSKchgtol = 1e-14;
    tsk->TSKvoltTol = 1e-6;
#ifdef NEWTRUNC
    tsk->TSKlteReltol = 1e-3;
    tsk->TSKlteAbstol = 1e-6;
#endif /* NEWTRUNC */

/* gtri - modify - 4/17/91 - wbk - Change trtol default */
/* Lower default value of trtol to give more accuracy */
/*    tsk->TSKtrtol = 7;  */
    tsk->TSKtrtol = 1;
/* gtri - modify - 4/17/91 - wbk - Change trtol default */

    tsk->TSKbypass = 1;
    tsk->TSKtranMaxIter = 10;
    tsk->TSKdcMaxIter = 100;
    tsk->TSKdcTrcvMaxIter = 50;
    tsk->TSKintegrateMethod = TRAPEZOIDAL;
    tsk->TSKmaxOrder = 2;
    tsk->TSKnumSrcSteps = 10;
    tsk->TSKnumGminSteps = 10;
    tsk->TSKpivotAbsTol = 1e-13;
    tsk->TSKpivotRelTol = 1e-3;
    tsk->TSKtemp = 300.15;
    tsk->TSKnomTemp = 300.15;
    tsk->TSKdefaultMosL = 1e-4;
    tsk->TSKdefaultMosW = 1e-4;
    tsk->TSKdefaultMosAD = 0;
    tsk->TSKdefaultMosAS = 0;
    tsk->TSKnoOpIter=0;
    return(OK);
}
