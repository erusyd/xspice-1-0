/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "IFsim.h"
#include "VCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:27 $")

/* ARGSUSED */
int
VCVSparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    VCVSinstance *here = (VCVSinstance *)inst;
    switch(param) {
        case VCVS_GAIN:
            here->VCVScoeff = value->rValue;
            here->VCVScoeffGiven = TRUE;
            break;
        case VCVS_GAIN_SENS:
            here->VCVSsenParmNo = value->iValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
