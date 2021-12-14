/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "URCdefs.h"
#include "util.h"
#include "IFsim.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("URCmParam.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:54 $")

int
URCmParam(param,value,inModel)
    int param;
    IFvalue *value;
    GENmodel *inModel;
{
    register URCmodel *model = (URCmodel *)inModel;
    switch(param) {
        case URC_MOD_K:
            model->URCk = value->rValue;
            model->URCkGiven = TRUE;
            break;
        case URC_MOD_FMAX:
            model->URCfmax = value->rValue;
            model->URCfmaxGiven = TRUE;
            break;
        case URC_MOD_RPERL:
            model->URCrPerL = value->rValue;
            model->URCrPerLGiven = TRUE;
            break;
        case URC_MOD_CPERL:
            model->URCcPerL = value->rValue;
            model->URCcPerLGiven = TRUE;
            break;
        case URC_MOD_ISPERL:
            model->URCisPerL = value->rValue;
            model->URCisPerLGiven = TRUE;
            break;
        case URC_MOD_RSPERL:
            model->URCrsPerL = value->rValue;
            model->URCrsPerLGiven = TRUE;
            break;
        case URC_MOD_URC:
            /* no operation - already know we are a URC, but this makes*/
            /* spice-2 like parsers happy */
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
