/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#include "CCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCVSparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:06 $")

/* ARGSUSED */
int
CCVSparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    CCVSinstance *here = (CCVSinstance*)inst;
    switch(param) {
        case CCVS_TRANS:
            here->CCVScoeff = value->rValue;
            here->CCVScoeffGiven = TRUE;
            break;
        case CCVS_CONTROL:
            here->CCVScontName = value->uValue;
            break;
        case CCVS_TRANS_SENS:
            here->CCVSsenParmNo = value->iValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
