/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "util.h"
#include "IFsim.h"
#include "RESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("RESparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:58 $")

/* ARGSUSED */
int
RESparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    RESinstance *here = (RESinstance *)inst;
    switch(param) {
        case RES_TEMP:
            here->REStemp = value->rValue+CONSTCtoK;
            here->REStempGiven = TRUE;
            break;
        case RES_RESIST:
            here->RESresist = value->rValue;
            here->RESresGiven = TRUE;
            break;
        case RES_WIDTH:
            here->RESwidth = value->rValue;
            here->RESwidthGiven = TRUE;
            break;
        case RES_LENGTH:
            here->RESlength = value->rValue;
            here->RESlengthGiven = TRUE;
            break;
        case RES_RESIST_SENS:
            here->RESsenParmNo = value->iValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
