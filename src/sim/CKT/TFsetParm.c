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
#include "TFdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("TFsetParm.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:59 $")

/* ARGSUSED */
int 
TFsetParm(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    case TF_OUTPOS:
        ((TFan *)anal)->TFoutPos = (CKTnode *)value->nValue;
        ((TFan *)anal)->TFoutIsV = TRUE;
        ((TFan *)anal)->TFoutIsI = FALSE;
        break;
    case TF_OUTNEG:
        ((TFan *)anal)->TFoutNeg = (CKTnode *)value->nValue;
        ((TFan *)anal)->TFoutIsV = TRUE;
        ((TFan *)anal)->TFoutIsI = FALSE;
        break;
    case TF_OUTNAME:
        ((TFan *)anal)->TFoutName = value->sValue;
        break;
    case TF_OUTSRC:
        ((TFan *)anal)->TFoutSrc = value->uValue;
        ((TFan *)anal)->TFoutIsV = FALSE;
        ((TFan *)anal)->TFoutIsI = TRUE;
        break;
    case TF_INSRC:
        ((TFan *)anal)->TFinSrc = value->uValue;
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}


static IFparm TFparms[] = {
    { "outpos",      TF_OUTPOS,     IF_SET|IF_NODE, "Positive output node" },
    { "outneg",      TF_OUTNEG,     IF_SET|IF_NODE, "Negative output node" },
    { "outname",     TF_OUTNAME,    IF_SET|IF_STRING,"Name of output variable"},
    { "outsrc",      TF_OUTSRC,     IF_SET|IF_INSTANCE, "Output source" },
    { "insrc",       TF_INSRC,      IF_SET|IF_INSTANCE, "Input source" }
};

SPICEanalysis TFinfo  = {
    { 
        "TF",
        "transfer function analysis",

        sizeof(TFparms)/sizeof(IFparm),
        TFparms
    },
    sizeof(TFan),
    TFsetParm,
    TFaskQuest
};
