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
#include "PZdefs.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("PZsetParm.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:46 $")

/* ARGSUSED */
int 
PZsetParm(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    case PZ_NODEI:
        ((PZAN*)anal)->PZnodeI = ((CKTnode*)value->nValue)->number;
        break;

    case PZ_NODEG:
        ((PZAN*)anal)->PZnodeG = ((CKTnode*)value->nValue)->number;
        break;

    case PZ_NODEJ:
        ((PZAN*)anal)->PZnodeJ = ((CKTnode*)value->nValue)->number;
        break;

    case PZ_NODEK:
        ((PZAN*)anal)->PZnodeK = ((CKTnode*)value->nValue)->number;
        break;

    case PZ_V:
        if(value->iValue) {
            ((PZAN*)anal)->PZflagVI = 0;
        }
        break;

    case PZ_I:
        if(value->iValue) {
            ((PZAN*)anal)->PZflagVI = 1;
        }
        break;

    case PZ_POL:
        if(value->iValue) {
            ((PZAN*)anal)->PZflagPZ = 1;
        }
        break;

    case PZ_ZER:
        if(value->iValue) {
            ((PZAN*)anal)->PZflagPZ = 2;
        }
        break;

    case PZ_PZ:
        if(value->iValue) {
            ((PZAN*)anal)->PZflagPZ = 3;
        }
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}


static IFparm PZparms[] = {
    { "nodei",  PZ_NODEI,   IF_SET|IF_ASK|IF_NODE,  "" },
    { "nodeg",  PZ_NODEG,   IF_SET|IF_ASK|IF_NODE,  "" },
    { "nodej",  PZ_NODEJ,   IF_SET|IF_ASK|IF_NODE,  "" },
    { "nodek",  PZ_NODEK,   IF_SET|IF_ASK|IF_NODE,  "" },
    { "vol",    PZ_V,       IF_SET|IF_ASK|IF_FLAG,  "" },
    { "cur",    PZ_I,       IF_SET|IF_ASK|IF_FLAG,  "" },
    { "pol",    PZ_POL,     IF_SET|IF_ASK|IF_FLAG,  "" },
    { "zer",    PZ_ZER,     IF_SET|IF_ASK|IF_FLAG,  "" },
    { "pz",     PZ_PZ,      IF_SET|IF_ASK|IF_FLAG,  "" }
};

SPICEanalysis PZinfo  = {
    { 
        "PZ",
        "pole-zero analysis",

        sizeof(PZparms)/sizeof(IFparm),
        PZparms
    },
    sizeof(PZAN),
    PZsetParm,
    PZaskQuest
};
