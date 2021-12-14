/*
 * Copyright (c) 1985 S. Hwang
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "IFsim.h"
#include "MESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MESmParam.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:06 $")

int
MESmParam(param,value,inModel)
    int param;
    IFvalue *value;
    GENmodel *inModel;
{
    MESmodel *model = (MESmodel*)inModel;
    switch(param) {
        case MES_MOD_VTO:
            model->MESthresholdGiven = TRUE;
            model->MESthreshold = value->rValue;
            break;
        case MES_MOD_ALPHA:
            model->MESalphaGiven = TRUE;
            model->MESalpha = value->rValue;
            break;
        case MES_MOD_BETA:
            model->MESbetaGiven = TRUE;
            model->MESbeta = value->rValue;
            break;
        case MES_MOD_LAMBDA:
            model->MESlModulationGiven = TRUE;
            model->MESlModulation = value->rValue;
            break;
        case MES_MOD_B:
            model->MESbGiven = TRUE;
            model->MESb = value->rValue;
            break;
        case MES_MOD_RD:
            model->MESdrainResistGiven = TRUE;
            model->MESdrainResist = value->rValue;
            break;
        case MES_MOD_RS:
            model->MESsourceResistGiven = TRUE;
            model->MESsourceResist = value->rValue;
            break;
        case MES_MOD_CGS:
            model->MEScapGSGiven = TRUE;
            model->MEScapGS = value->rValue;
            break;
        case MES_MOD_CGD:
            model->MEScapGDGiven = TRUE;
            model->MEScapGD = value->rValue;
            break;
        case MES_MOD_PB:
            model->MESgatePotentialGiven = TRUE;
            model->MESgatePotential = value->rValue;
            break;
        case MES_MOD_IS:
            model->MESgateSatCurrentGiven = TRUE;
            model->MESgateSatCurrent = value->rValue;
            break;
        case MES_MOD_FC:
            model->MESdepletionCapCoeffGiven = TRUE;
            model->MESdepletionCapCoeff = value->rValue;
            break;
        case MES_MOD_NMF:
            if(value->iValue) {
                model->MEStype = NMF;
            }
            break;
        case MES_MOD_PMF:
            if(value->iValue) {
                model->MEStype = PMF;
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
