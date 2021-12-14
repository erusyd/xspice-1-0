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
#include "TRCVdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("DCTsetParm.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:35 $")

/* ARGSUSED */
int 
DCTsetParm(ckt,anal,which,value)
    CKTcircuit  *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    register TRCV* cv= (TRCV*)anal;
    switch(which) {

    case DCT_START1:
        cv->TRCVvStart[0] = value->rValue;
        cv->TRCVnestLevel = MAX(0,cv->TRCVnestLevel);
        cv->TRCVset[0]=TRUE;
        break;

    case DCT_STOP1:
        cv->TRCVvStop[0] = value->rValue;
        cv->TRCVnestLevel = MAX(0,cv->TRCVnestLevel);
        cv->TRCVset[0]=TRUE;
        break;

    case DCT_STEP1:
        cv->TRCVvStep[0] = value->rValue;
        cv->TRCVnestLevel = MAX(0,cv->TRCVnestLevel);
        cv->TRCVset[0]=TRUE;
        break;

    case DCT_START2:
        cv->TRCVvStart[1] = value->rValue;
        cv->TRCVnestLevel = MAX(1,cv->TRCVnestLevel);
        cv->TRCVset[1]=TRUE;
        break;

    case DCT_STOP2:
        cv->TRCVvStop[1] = value->rValue;
        cv->TRCVnestLevel = MAX(1,cv->TRCVnestLevel);
        cv->TRCVset[1]=TRUE;
        break;

    case DCT_STEP2:
        cv->TRCVvStep[1] = value->rValue;
        cv->TRCVnestLevel = MAX(1,cv->TRCVnestLevel);
        cv->TRCVset[1]=TRUE;
        break;
    
    case DCT_NAME1:
        cv->TRCVvName[0] = value->uValue;
        cv->TRCVnestLevel = MAX(0,cv->TRCVnestLevel);
        cv->TRCVset[0]=TRUE;
        break;

    case DCT_NAME2:
        cv->TRCVvName[1] = value->uValue;
        cv->TRCVnestLevel = MAX(1,cv->TRCVnestLevel);
        cv->TRCVset[1]=TRUE;
        break;

    case DCT_TYPE1:
        cv->TRCVvType[0] = value->iValue;
        cv->TRCVnestLevel = MAX(0,cv->TRCVnestLevel);
        cv->TRCVset[0]=TRUE;
        break;

    case DCT_TYPE2:
        cv->TRCVvType[1] = value->iValue;
        cv->TRCVnestLevel = MAX(1,cv->TRCVnestLevel);
        cv->TRCVset[1]=TRUE;
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}


static IFparm DCTparms[] = {
    { "start1",     DCT_START1, IF_SET|IF_REAL,     "starting voltage/current"},
    { "stop1",      DCT_STOP1,  IF_SET|IF_REAL,     "ending voltage/current" },
    { "step1",      DCT_STEP1,  IF_SET|IF_REAL,     "voltage/current step" },
    { "start2",     DCT_START2, IF_SET|IF_REAL,     "starting voltage/current"},
    { "stop2",      DCT_STOP2,  IF_SET|IF_REAL,     "ending voltage/current" },
    { "step2",      DCT_STEP2,  IF_SET|IF_REAL,     "voltage/current step" },
    { "name1",      DCT_NAME1,  IF_SET|IF_INSTANCE, "name of source to step" },
    { "name2",      DCT_NAME2,  IF_SET|IF_INSTANCE, "name of source to step" },
    { "type1",      DCT_TYPE1,  IF_SET|IF_INTEGER,  "type of source to step" },
    { "type2",      DCT_TYPE2,  IF_SET|IF_INTEGER,  "type of source to step" }
};

SPICEanalysis DCTinfo  = {
    { 
        "DCTransfer",
        "D.C. Transfer curve analysis",

        sizeof(DCTparms)/sizeof(IFparm),
        DCTparms
    },
    sizeof(TRCV),
    DCTsetParm,
    DCTaskQuest
};
