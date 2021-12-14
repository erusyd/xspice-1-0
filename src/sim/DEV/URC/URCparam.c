/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#include "URCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("URCparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:55 $")

/* ARGSUSED */
int
URCparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    URCinstance *here = (URCinstance *)inst;
    switch(param) {
        case URC_LEN:
            here->URClength = value->rValue;
            here->URClenGiven = TRUE;
            break;
        case URC_LUMPS:
            here->URClumps = value->rValue;
            here->URClumpsGiven = TRUE;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
