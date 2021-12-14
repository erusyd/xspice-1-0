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

RCSID("TRANsetParm.c $Revision: 1.1 $ on $Date: 91/04/02 12:09:02 $")

/* ARGSUSED */
int 
TRANsetParm(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    case TRAN_TSTOP:
        ((TRANan *)anal)->TRANfinalTime = value->rValue;
        break;
    case TRAN_TSTEP:
        ((TRANan *)anal)->TRANstep = value->rValue;
        break;
    case TRAN_TSTART:
        ((TRANan *)anal)->TRANinitTime = value->rValue;
        break;
    case TRAN_TMAX:
        ((TRANan *)anal)->TRANmaxStep = value->rValue;
        break;
    case TRAN_UIC:
        if(value->iValue) {
            ((TRANan *)anal)->TRANmode |= MODEUIC;
        }
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}


static IFparm TRANparms[] = {
    { "tstart",     TRAN_TSTART,    IF_SET|IF_REAL, "starting time" },
    { "tstop",      TRAN_TSTOP,     IF_SET|IF_REAL, "ending time" },
    { "tstep",      TRAN_TSTEP,     IF_SET|IF_REAL, "time step" },
    { "tmax",       TRAN_TMAX,      IF_SET|IF_REAL, "maximum time step" },
    { "uic",        TRAN_UIC,       IF_SET|IF_FLAG, "use initial conditions" },
};

SPICEanalysis TRANinfo  = {
    { 
        "TRAN",
        "Transient analysis",

        sizeof(TRANparms)/sizeof(IFparm),
        TRANparms
    },
    sizeof(TRANan),
    TRANsetParm,
    TRANaskQuest
};
