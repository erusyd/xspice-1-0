/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "CKTdefs.h"
#include "IFsim.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:39 $")

/* ARGSUSED */
int
DIOmAsk (ckt,inModel,which, value)
    CKTcircuit *ckt;
    int which;
    IFvalue *value;
    GENmodel *inModel;
{
    DIOmodel *model = (DIOmodel*)inModel;
    switch (which) {
        case DIO_MOD_IS:
            value->rValue = model->DIOsatCur;
            return(OK);
        case DIO_MOD_TNOM:
            value->rValue = model->DIOnomTemp-CONSTCtoK;
            return(OK);
        case DIO_MOD_RS:
            value->rValue = model->DIOresist;
            return(OK);
        case DIO_MOD_N:
            value->rValue = model->DIOemissionCoeff;
            return(OK);
        case DIO_MOD_TT:
            value->rValue = model->DIOtransitTime;
            return(OK);
        case DIO_MOD_CJO:
            value->rValue = model->DIOjunctionCap;
            return(OK);
        case DIO_MOD_VJ:
            value->rValue = model->DIOjunctionPot;
            return(OK);
        case DIO_MOD_M:
            value->rValue = model->DIOgradingCoeff;
            return(OK);
        case DIO_MOD_EG:
            value->rValue = model->DIOactivationEnergy;
            return (OK);
        case DIO_MOD_XTI:
            value->rValue = model->DIOsaturationCurrentExp;
            return(OK);
        case DIO_MOD_FC:
            value->rValue = model->DIOdepletionCapCoeff;
            return(OK);
        case DIO_MOD_BV:
            value->rValue = model->DIObreakdownVoltage;
            return(OK);
        case DIO_MOD_IBV:
            value->rValue = model->DIObreakdownCurrent;
            return(OK);
        case DIO_MOD_COND:
            value->rValue = model->DIOconductance;
            return(OK);
        default:
            return(E_BADPARM);
        }
}

