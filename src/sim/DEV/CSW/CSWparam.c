/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "CSWdefs.h"
#include "IFsim.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CSWparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:23 $")

/* ARGSUSED */
int
CSWparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    CSWinstance *here = (CSWinstance*)inst;
    switch(param) {
        case  CSW_CONTROL:
            here->CSWcontName = value->uValue;
            break;
        case CSW_IC_ON:
            if(value->iValue) {
                here->CSWzero_stateGiven = TRUE;
            }
            break;
        case CSW_IC_OFF:
            if(value->iValue) {
                here->CSWzero_stateGiven = FALSE;
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
