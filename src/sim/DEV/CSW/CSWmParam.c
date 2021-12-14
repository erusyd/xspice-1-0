/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "CSWdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CSWmParam.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:22 $")

int
CSWmParam(param,value,inModel)
    int param;
    IFvalue *value;
    GENmodel *inModel;
{
    CSWmodel *model = (CSWmodel*)inModel;
    switch(param) {
        case CSW_CSW:
            /* just says that this is a switch */
            break;
        case CSW_RON:
            model->CSWonResistance = value->rValue;
            model->CSWonConduct = 1.0/(value->rValue);
            model->CSWonGiven = TRUE;
            break;
        case CSW_ROFF:
            model->CSWoffResistance = value->rValue;
            model->CSWoffConduct = 1.0/(value->rValue);
            model->CSWoffGiven = TRUE;
            break;
        case CSW_ITH:
            model->CSWiThreshold = value->rValue;
            model->CSWthreshGiven = TRUE;
            break;
        case CSW_IHYS:
            /* take absolute value of hysteresis voltage */
            model->CSWiHysteresis = FABS(value->rValue);
            model->CSWhystGiven = TRUE;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
