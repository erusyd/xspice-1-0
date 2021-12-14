/*
 * Copyright (c) 1987 Mathew Lew and Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "IFsim.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "JFETdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("JFETmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:47 $")

/*ARGSUSED*/
int
JFETmAsk(ckt,inModel,which,value)
    CKTcircuit *ckt;
    GENmodel *inModel;
    int which;
    IFvalue *value;
{
    JFETmodel *model = (JFETmodel*)inModel;
    switch(which) {
        case JFET_MOD_TNOM:
            value->rValue = model->JFETtnom-CONSTCtoK;
            return(OK);
        case JFET_MOD_VTO:
            value->rValue = model->JFETthreshold;
            return(OK);
        case JFET_MOD_BETA:
            value->rValue = model->JFETbeta;
            return(OK);
        case JFET_MOD_LAMBDA:
            value->rValue = model->JFETlModulation;
            return(OK);
        case JFET_MOD_RD:
            value->rValue = model->JFETdrainResist;
            return(OK);
        case JFET_MOD_RS:
            value->rValue = model->JFETsourceResist;
            return(OK);
        case JFET_MOD_CGS:
            value->rValue = model->JFETcapGS;
            return(OK);
        case JFET_MOD_CGD:
            value->rValue = model->JFETcapGD;
            return(OK);
        case JFET_MOD_PB:
            value->rValue = model->JFETgatePotential;
            return(OK);
        case JFET_MOD_IS:
            value->rValue = model->JFETgateSatCurrent;
            return(OK);
        case JFET_MOD_FC:
            value->rValue = model->JFETdepletionCapCoeff;
            return(OK);
        case JFET_MOD_DRAINCONDUCT:
            value->rValue = model->JFETdrainConduct;
            return(OK);
        case JFET_MOD_SOURCECONDUCT:
            value->rValue = model->JFETsourceConduct;
            return(OK);
        default:
            return(E_BADPARM);
    }
    /* NOTREACHED */
}

