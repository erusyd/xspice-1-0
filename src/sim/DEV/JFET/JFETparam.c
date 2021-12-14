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

RCSID("JFETparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:49 $")

/* ARGSUSED */
int
JFETparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    JFETinstance *here = (JFETinstance *)inst;
    switch(param) {
        case JFET_TEMP:
            here->JFETtemp = value->rValue+CONSTCtoK;
            here->JFETtempGiven = TRUE;
            break;
        case JFET_AREA:
            here->JFETarea = value->rValue;
            here->JFETareaGiven = TRUE;
            break;
        case JFET_IC_VDS:
            here->JFETicVDS = value->rValue;
            here->JFETicVDSGiven = TRUE;
            break;
        case JFET_IC_VGS:
            here->JFETicVGS = value->rValue;
            here->JFETicVGSGiven = TRUE;
            break;
        case JFET_OFF:
            here->JFEToff = value->iValue;
            break;
        case JFET_IC:
            switch(value->v.numValue) {
                case 2:
                    here->JFETicVGS = *(value->v.vec.rVec+1);
                    here->JFETicVGSGiven = TRUE;
                case 1:
                    here->JFETicVDS = *(value->v.vec.rVec);
                    here->JFETicVDSGiven = TRUE;
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
