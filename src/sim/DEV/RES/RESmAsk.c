/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "CKTdefs.h"
#include "IFsim.h"
#include "RESdefs.h"
#include "SPerror.h"
#include "DEVdefs.h"
#include "suffix.h"

RCSID("RESmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:55 $")

/* ARGSUSED */
int 
RESmodAsk(ckt,inModel,which,value)
    CKTcircuit *ckt;
    GENmodel *inModel;
    int which;
    IFvalue *value;
{
    RESmodel *model = (RESmodel *)inModel;
    switch(which) {
        case RES_MOD_TNOM:
            value->rValue = model->REStnom-CONSTCtoK;
            return(OK);
        case RES_MOD_TC1:
            value->rValue = model->REStempCoeff1;
            return(OK);
        case RES_MOD_TC2:
            value->rValue = model->REStempCoeff2;
            return(OK);
        case RES_MOD_RSH:
            value->rValue = model->RESsheetRes;
            return(OK);
        case RES_MOD_DEFWIDTH:
            value->rValue = model->RESdefWidth;
            return(OK);
        case RES_MOD_NARROW: 
            value->rValue = model->RESnarrow;
            return(OK);
        default:
            return(E_BADPARM);
    }
}

