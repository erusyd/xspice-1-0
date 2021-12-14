/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "VSRCdefs.h"
#include "IFsim.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VSRCparam.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:49 $")

/* ARGSUSED */
int
VSRCparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    VSRCinstance *here = (VSRCinstance *)inst;
    switch(param) {
        case VSRC_DC:
            here->VSRCdcValue = value->rValue;
            here->VSRCdcGiven = TRUE;
            break;
        case VSRC_AC_MAG:
            here->VSRCacMag = value->rValue;
            here->VSRCacMGiven = TRUE;
            here->VSRCacGiven = TRUE;
            break;
        case VSRC_AC_PHASE:
            here->VSRCacPhase = value->rValue;
            here->VSRCacPGiven = TRUE;
            here->VSRCacGiven = TRUE;
            break;
        case VSRC_AC:
            switch(value->v.numValue) {
                case 2:
                    here->VSRCacPhase = *(value->v.vec.rVec+1);
                    here->VSRCacPGiven = TRUE;
                case 1:
                    here->VSRCacMag = *(value->v.vec.rVec);
                    here->VSRCacMGiven = TRUE;
                case 0:
                    here->VSRCacGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case VSRC_PULSE:
            here->VSRCfunctionType = PULSE;
            here->VSRCfuncTGiven = TRUE;
            here->VSRCcoeffs = value->v.vec.rVec;
            here->VSRCfunctionOrder = value->v.numValue;
            here->VSRCcoeffsGiven = TRUE;
            break;
        case VSRC_SINE:
            here->VSRCfunctionType = SINE;
            here->VSRCfuncTGiven = TRUE;
            here->VSRCcoeffs = value->v.vec.rVec;
            here->VSRCfunctionOrder = value->v.numValue;
            here->VSRCcoeffsGiven = TRUE;
            break;
        case VSRC_EXP:
            here->VSRCfunctionType = EXP;
            here->VSRCfuncTGiven = TRUE;
            here->VSRCcoeffs = value->v.vec.rVec;
            here->VSRCfunctionOrder = value->v.numValue;
            here->VSRCcoeffsGiven = TRUE;
            break;
        case VSRC_PWL:
            here->VSRCfunctionType = PWL;
            here->VSRCfuncTGiven = TRUE;
            here->VSRCcoeffs = value->v.vec.rVec;
            here->VSRCfunctionOrder = value->v.numValue;
            here->VSRCcoeffsGiven = TRUE;
            break;
        case VSRC_SFFM:
            here->VSRCfunctionType = SFFM;
            here->VSRCfuncTGiven = TRUE;
            here->VSRCcoeffs = value->v.vec.rVec;
            here->VSRCfunctionOrder = value->v.numValue;
            here->VSRCcoeffsGiven = TRUE;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
