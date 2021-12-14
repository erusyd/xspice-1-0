/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "BSIMdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:11 $")

/* ARGSUSED */
int
BSIMparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    BSIMinstance *here = (BSIMinstance*)inst;
    switch(param) {
        case BSIM_W:
            here->BSIMw = value->rValue;
            here->BSIMwGiven = TRUE;
            break;
        case BSIM_L:
            here->BSIMl = value->rValue;
            here->BSIMlGiven = TRUE;
            break;
        case BSIM_AS:
            here->BSIMsourceArea = value->rValue;
            here->BSIMsourceAreaGiven = TRUE;
            break;
        case BSIM_AD:
            here->BSIMdrainArea = value->rValue;
            here->BSIMdrainAreaGiven = TRUE;
            break;
        case BSIM_PS:
            here->BSIMsourcePerimeter = value->rValue;
            here->BSIMsourcePerimeterGiven = TRUE;
            break;
        case BSIM_PD:
            here->BSIMdrainPerimeter = value->rValue;
            here->BSIMdrainPerimeterGiven = TRUE;
            break;
        case BSIM_NRS:
            here->BSIMsourceSquares = value->rValue;
            here->BSIMsourceSquaresGiven = TRUE;
            break;
        case BSIM_NRD:
            here->BSIMdrainSquares = value->rValue;
            here->BSIMdrainSquaresGiven = TRUE;
            break;
        case BSIM_OFF:
            here->BSIMoff = value->iValue;
            break;
        case BSIM_IC_VBS:
            here->BSIMicVBS = value->rValue;
            here->BSIMicVBSGiven = TRUE;
            break;
        case BSIM_IC_VDS:
            here->BSIMicVDS = value->rValue;
            here->BSIMicVDSGiven = TRUE;
            break;
        case BSIM_IC_VGS:
            here->BSIMicVGS = value->rValue;
            here->BSIMicVGSGiven = TRUE;
            break;
        case BSIM_IC:
            switch(value->v.numValue){
                case 3:
                    here->BSIMicVBS = *(value->v.vec.rVec+2);
                    here->BSIMicVBSGiven = TRUE;
                case 2:
                    here->BSIMicVGS = *(value->v.vec.rVec+1);
                    here->BSIMicVGSGiven = TRUE;
                case 1:
                    here->BSIMicVDS = *(value->v.vec.rVec);
                    here->BSIMicVDSGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}


