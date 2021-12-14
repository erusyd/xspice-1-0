/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "IFsim.h"
#include "util.h"
#include "JFETdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("JFETmParam.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:48 $")

int
JFETmParam(param,value,inModels)
    int param;
    IFvalue *value;
    GENmodel *inModels;
{
    JFETmodel *model = (JFETmodel*)inModels;
    switch(param) {
        case JFET_MOD_TNOM:
            model->JFETtnomGiven = TRUE+CONSTCtoK;
            model->JFETtnom = value->rValue;
            break;
        case JFET_MOD_VTO:
            model->JFETthresholdGiven = TRUE;
            model->JFETthreshold = value->rValue;
            break;
        case JFET_MOD_BETA:
            model->JFETbetaGiven = TRUE;
            model->JFETbeta = value->rValue;
            break;
        case JFET_MOD_LAMBDA:
            model->JFETlModulationGiven = TRUE;
            model->JFETlModulation = value->rValue;
            break;
        case JFET_MOD_RD:
            model->JFETdrainResistGiven = TRUE;
            model->JFETdrainResist = value->rValue;
            break;
        case JFET_MOD_RS:
            model->JFETsourceResistGiven = TRUE;
            model->JFETsourceResist = value->rValue;
            break;
        case JFET_MOD_CGS:
            model->JFETcapGSGiven = TRUE;
            model->JFETcapGS = value->rValue;
            break;
        case JFET_MOD_CGD:
            model->JFETcapGDGiven = TRUE;
            model->JFETcapGD = value->rValue;
            break;
        case JFET_MOD_PB:
            model->JFETgatePotentialGiven = TRUE;
            model->JFETgatePotential = value->rValue;
            break;
        case JFET_MOD_IS:
            model->JFETgateSatCurrentGiven = TRUE;
            model->JFETgateSatCurrent = value->rValue;
            break;
        case JFET_MOD_FC:
            model->JFETdepletionCapCoeffGiven = TRUE;
            model->JFETdepletionCapCoeff = value->rValue;
            break;
        case JFET_MOD_NJF:
            if(value->iValue) {
                model->JFETtype = NJF;
            }
            break;
        case JFET_MOD_PJF:
            if(value->iValue) {
                model->JFETtype = PJF;
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
