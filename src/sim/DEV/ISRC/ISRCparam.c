/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "IFsim.h"
#include "ISRCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("ISRCparam.c $Revision: 1.1 $ on $Date: 91/07/11 11:06:36 $")

/* ARGSUSED */
int
ISRCparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    ISRCinstance *here = (ISRCinstance*)inst;
    switch(param) {
        case ISRC_DC:
            here->ISRCdcValue = value->rValue;
            here->ISRCdcGiven = TRUE;
            break;
        case ISRC_AC_MAG:
            here->ISRCacMag = value->rValue;
            here->ISRCacMGiven = TRUE;
            here->ISRCacGiven = TRUE;
            break;
        case ISRC_AC_PHASE:
            here->ISRCacPhase = value->rValue;
            here->ISRCacPGiven = TRUE;
            here->ISRCacGiven = TRUE;
            break;
        case ISRC_AC:
            switch(value->v.numValue) {
                case 2:
                    here->ISRCacPhase = *(value->v.vec.rVec+1);
                    here->ISRCacPGiven = TRUE;
                case 1:
                    here->ISRCacMag = *(value->v.vec.rVec);
                    here->ISRCacMGiven = TRUE;
                case 0:
                    here->ISRCacGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case ISRC_PULSE:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = PULSE;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        case ISRC_SINE:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = SINE;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        case ISRC_EXP:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = EXP;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        case ISRC_PWL:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = PWL;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        case ISRC_SFFM:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = SFFM;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
