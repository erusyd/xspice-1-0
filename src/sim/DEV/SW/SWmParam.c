/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "SWdefs.h"
#include "util.h"
#include "IFsim.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SWmParam.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:24 $")

int
SWmParam(param,value,inModel)
    int param;
    IFvalue *value;
    GENmodel *inModel;
{
    SWmodel *model = (SWmodel *)inModel;
    switch(param) {
        case SW_MOD_SW:
            /* just says that this is a switch */
            break;
        case SW_MOD_RON:
            model->SWonResistance = value->rValue;
            model->SWonConduct = 1.0/(value->rValue);
            model->SWonGiven = TRUE;
            break;
        case SW_MOD_ROFF:
            model->SWoffResistance = value->rValue;
            model->SWoffConduct = 1.0/(value->rValue);
            model->SWoffGiven = TRUE;
            break;
        case SW_MOD_VTH:
            /* take absolute value of hysteresis voltage */
            model->SWvThreshold = value->rValue;
            model->SWthreshGiven = TRUE;
            break;
        case SW_MOD_VHYS:
            /* take absolute value of hysteresis voltage */
            model->SWvHysteresis = (value->rValue < 0) ? -(value->rValue) :
                    value->rValue;
            model->SWhystGiven = TRUE;
            break;
        default:
            return(E_BADPARM);
    }

    return(OK);
}
