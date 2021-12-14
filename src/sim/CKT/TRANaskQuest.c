/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "TRANdefs.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("TRANaskQuest.c $Revision: 1.1 $ on $Date: 91/04/02 12:09:01 $")

/* ARGSUSED */
int 
TRANaskQuest(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    case TRAN_TSTOP:
        value->rValue = ((TRANan *)anal)->TRANfinalTime;
        break;
    case TRAN_TSTEP:
        value->rValue = ((TRANan *)anal)->TRANstep;
        break;
    case TRAN_TSTART:
        value->rValue = ((TRANan *)anal)->TRANinitTime;
        break;
    case TRAN_TMAX:
        value->rValue = ((TRANan *)anal)->TRANmaxStep;
        break;
    case TRAN_UIC:
        if(((TRANan *)anal)->TRANmode & MODEUIC) {
            value->iValue = 1;
        } else {
            value->iValue = 0;
        }
        break;


    default:
        return(E_BADPARM);
    }
    return(OK);
}

