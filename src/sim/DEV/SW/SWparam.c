/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "SWdefs.h"
#include "IFsim.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SWparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:24 $")

/*ARGSUSED*/
int
SWparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    SWinstance *here = (SWinstance *)inst;
    switch(param) {
        case SW_IC_ON:
            if(value->iValue) {
                here->SWzero_stateGiven = TRUE;
            }
            break;
        case SW_IC_OFF:
            if(value->iValue) {
                here->SWzero_stateGiven = FALSE;
            }
            break;
        default:
            return(E_BADPARM);
    }

    return(OK);
}
