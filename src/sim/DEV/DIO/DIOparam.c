/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "IFsim.h"
#include "util.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:41 $")

/* ARGSUSED */
int
DIOparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    DIOinstance *here = (DIOinstance*)inst;
    switch(param) {
        case DIO_AREA:
            here->DIOarea = value->rValue;
            here->DIOareaGiven = TRUE;
            break;
        case DIO_TEMP:
            here->DIOtemp = value->rValue+CONSTCtoK;
            here->DIOtempGiven = TRUE;
            break;
        case DIO_OFF:
            here->DIOoff = value->iValue;
            break;
        case DIO_IC:
            here->DIOinitCond = value->rValue;
            break;
        case DIO_AREA_SENS:
            here->DIOsenParmNo = value->iValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
