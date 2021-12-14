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

RCSID("RESmParam.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:57 $")

int
RESmParam(param,value,inModel)
    int param;
    IFvalue *value;
    GENmodel *inModel;
{
    register RESmodel *model = (RESmodel *)inModel;
    switch(param) {
        case RES_MOD_TNOM:
            model->REStnom = value->rValue+CONSTCtoK;
            model->REStnomGiven = TRUE;
            break;
        case RES_MOD_TC1:
            model->REStempCoeff1 = value->rValue;
            model->REStc1Given = TRUE;
            break;
        case RES_MOD_TC2:
            model->REStempCoeff2 = value->rValue;
            model->REStc2Given = TRUE;
            break;
        case RES_MOD_RSH:
            model->RESsheetRes = value->rValue;
            model->RESsheetResGiven = TRUE;
            break;
        case RES_MOD_DEFWIDTH:
            model->RESdefWidth = value->rValue;
            model->RESdefWidthGiven = TRUE;
            break;
        case RES_MOD_NARROW:
            model->RESnarrow = value->rValue;
            model->RESnarrowGiven = TRUE;
            break;
        case RES_MOD_R:
            /* just being reassured by user that this is a resistor model */
            /* no-op */
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
