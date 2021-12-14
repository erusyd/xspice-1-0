/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "util.h"
#include "IFsim.h"
#include "MOS2defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS2param.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:58 $")

/* ARGSUSED */
int
MOS2param(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    MOS2instance *here = (MOS2instance *)inst;
    switch(param) {
        case MOS2_TEMP:
            here->MOS2temp = value->rValue+CONSTCtoK;
            here->MOS2tempGiven = TRUE;
            break;
        case MOS2_W:
            here->MOS2w = value->rValue;
            here->MOS2wGiven = TRUE;
            break;
        case MOS2_L:
            here->MOS2l = value->rValue;
            here->MOS2lGiven = TRUE;
            break;
        case MOS2_AS:
            here->MOS2sourceArea = value->rValue;
            here->MOS2sourceAreaGiven = TRUE;
            break;
        case MOS2_AD:
            here->MOS2drainArea = value->rValue;
            here->MOS2drainAreaGiven = TRUE;
            break;
        case MOS2_PS:
            here->MOS2sourcePerimiter = value->rValue;
            here->MOS2sourcePerimiterGiven = TRUE;
            break;
        case MOS2_PD:
            here->MOS2drainPerimiter = value->rValue;
            here->MOS2drainPerimiterGiven = TRUE;
            break;
        case MOS2_NRS:
            here->MOS2sourceSquares = value->rValue;
            here->MOS2sourceSquaresGiven = TRUE;
            break;
        case MOS2_NRD:
            here->MOS2drainSquares = value->rValue;
            here->MOS2drainSquaresGiven = TRUE;
            break;
        case MOS2_OFF:
            here->MOS2off = value->iValue;
            break;
        case MOS2_IC_VBS:
            here->MOS2icVBS = value->rValue;
            here->MOS2icVBSGiven = TRUE;
            break;
        case MOS2_IC_VDS:
            here->MOS2icVDS = value->rValue;
            here->MOS2icVDSGiven = TRUE;
            break;
        case MOS2_IC_VGS:
            here->MOS2icVGS = value->rValue;
            here->MOS2icVGSGiven = TRUE;
            break;
        case MOS2_IC:
            switch(value->v.numValue){
                case 3:
                    here->MOS2icVBS = *(value->v.vec.rVec+2);
                    here->MOS2icVBSGiven = TRUE;
                case 2:
                    here->MOS2icVGS = *(value->v.vec.rVec+1);
                    here->MOS2icVGSGiven = TRUE;
                case 1:
                    here->MOS2icVDS = *(value->v.vec.rVec);
                    here->MOS2icVDSGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case MOS2_L_SENS:
            if(value->iValue) {
                here->MOS2senParmNo = 1;
                here->MOS2sens_l = 1;
            }
            break;
        case MOS2_W_SENS:
            if(value->iValue) {
                here->MOS2senParmNo = 1;
                here->MOS2sens_w = 1;
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
