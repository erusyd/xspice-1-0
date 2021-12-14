/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#include "CCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCCSparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:49 $")

/* ARGSUSED */
int
CCCSparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    CCCSinstance *here = (CCCSinstance*)inst;
    switch(param) {
        case CCCS_GAIN:
            here->CCCScoeff = value->rValue;
            here->CCCScoeffGiven = TRUE;
            break;
        case CCCS_CONTROL:
            here->CCCScontName = value->uValue;
            break;
        case CCCS_GAIN_SENS:
            here->CCCSsenParmNo = value->iValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
