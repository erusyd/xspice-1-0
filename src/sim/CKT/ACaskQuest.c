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

RCSID("ACaskQuest.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:00 $")
/* ARGSUSED */
int 
ACaskQuest(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    case AC_START:
        value->rValue = ((ACAN*)anal)->ACstartFreq;
        break;

    case AC_STOP:
        value->rValue = ((ACAN*)anal)->ACstopFreq ;
        break;

    case AC_STEPS:
        value->iValue = ((ACAN*)anal)->ACnumberSteps;
        break;

    case AC_DEC:
        if(((ACAN*)anal)->ACstepType == DECADE) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    case AC_OCT:
        if(((ACAN*)anal)->ACstepType == OCTAVE) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    case AC_LIN:
        if(((ACAN*)anal)->ACstepType == LINEAR) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}

