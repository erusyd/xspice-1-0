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
#include "ACdefs.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("ACsetParm.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:02 $")

/* ARGSUSED */
int 
ACsetParm(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    case AC_START:
        ((ACAN*)anal)->ACstartFreq = value->rValue;
        break;

    case AC_STOP:
        ((ACAN*)anal)->ACstopFreq = value->rValue;
        break;

    case AC_STEPS:
        ((ACAN*)anal)->ACnumberSteps = value->iValue;
        break;

    case AC_DEC:
        if(value->iValue) {
            ((ACAN*)anal)->ACstepType = DECADE;
        } else {
            if( ((ACAN*)anal)->ACstepType == DECADE) {
                ((ACAN*)anal)->ACstepType = 0;
            }
        }
        break;

    case AC_OCT:
        if(value->iValue) {
                ((ACAN*)anal)->ACstepType = OCTAVE;
        } else {
            if( ((ACAN*)anal)->ACstepType == OCTAVE) {
                ((ACAN*)anal)->ACstepType = 0;
            }
        }
        break;

    case AC_LIN:
        if(value->iValue) {
            ((ACAN*)anal)->ACstepType = LINEAR;
        } else {
            if( ((ACAN*)anal)->ACstepType == LINEAR) {
                ((ACAN*)anal)->ACstepType = 0;
            }
        }
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}


static IFparm ACparms[] = {
    { "start",      AC_START,   IF_SET|IF_ASK|IF_REAL, "starting frequency" },
    { "stop",       AC_STOP,    IF_SET|IF_ASK|IF_REAL, "ending frequency" },
    { "numsteps",   AC_STEPS,IF_SET|IF_ASK|IF_INTEGER, "number of frequencies"},
    { "dec",        AC_DEC,     IF_SET|IF_FLAG, "step by decades" },
    { "oct",        AC_OCT,     IF_SET|IF_FLAG, "step by octaves" },
    { "lin",        AC_LIN,     IF_SET|IF_FLAG, "step linearly" }
};

SPICEanalysis ACinfo  = {
    { 
        "AC",
        "A.C. Small signal analysis",

        sizeof(ACparms)/sizeof(IFparm),
        ACparms
    },
    sizeof(ACAN),
    ACsetParm,
    ACaskQuest
};
