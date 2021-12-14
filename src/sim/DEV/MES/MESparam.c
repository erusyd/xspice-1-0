/*
 * Copyright (c) 1985 S. Hwang
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#include "MESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MESparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:07 $")

/* ARGSUSED */
int
MESparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    MESinstance *here = (MESinstance*)inst;
    switch(param) {
        case MES_AREA:
            here->MESarea = value->rValue;
            here->MESareaGiven = TRUE;
            break;
        case MES_IC_VDS:
            here->MESicVDS = value->rValue;
            here->MESicVDSGiven = TRUE;
            break;
        case MES_IC_VGS:
            here->MESicVGS = value->rValue;
            here->MESicVGSGiven = TRUE;
            break;
        case MES_OFF:
            here->MESoff = value->iValue;
            break;
        case MES_IC:
            switch(value->v.numValue) {
                case 2:
                    here->MESicVGS = *(value->v.vec.rVec+1);
                    here->MESicVGSGiven = TRUE;
                case 1:
                    here->MESicVDS = *(value->v.vec.rVec);
                    here->MESicVDSGiven = TRUE;
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
