/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "IFsim.h"
#include "VCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCCSparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:07 $")

/* ARGSUSED */
int
VCCSparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    VCCSinstance *here = (VCCSinstance *)inst;
    switch(param) {
        case VCCS_TRANS:
            here->VCCScoeff = value->rValue;
            here->VCCScoeffGiven = TRUE;
            break;
        case VCCS_TRANS_SENS:
            here->VCCSsenParmNo = value->iValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
