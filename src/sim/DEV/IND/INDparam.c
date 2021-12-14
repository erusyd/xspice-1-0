/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("INDparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:02 $")

/* ARGSUSED */
int
INDparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    INDinstance *here = (INDinstance*)inst;
    switch(param) {
        case IND_IND:
            here->INDinduct = value->rValue;
            here->INDindGiven = TRUE;
            break;
        case IND_IC:
            here->INDinitCond = value->rValue;
            here->INDicGiven = TRUE;
            break;
        case IND_IND_SENS:
            here->INDsenParmNo = value->iValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
