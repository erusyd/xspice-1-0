/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*
 * This routine sets instance parameters for
 * BJTs in the circuit.
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "util.h"
#include "IFsim.h"
#include "BJTdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BJTparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:44 $")

/* ARGSUSED */
int
BJTparam(param,value,instPtr,select)
    int param;
    IFvalue *value;
    GENinstance *instPtr;
    IFvalue *select;
{
    register BJTinstance *here = (BJTinstance*)instPtr;

    switch(param) {
        case BJT_AREA:
            here->BJTarea = value->rValue;
            here->BJTareaGiven = TRUE;
            break;
        case BJT_TEMP:
            here->BJTtemp = value->rValue+CONSTCtoK;
            here->BJTtempGiven = TRUE;
            break;
        case BJT_OFF:
            here->BJToff = value->iValue;
            break;
        case BJT_IC_VBE:
            here->BJTicVBE = value->rValue;
            here->BJTicVBEGiven = TRUE;
            break;
        case BJT_IC_VCE:
            here->BJTicVCE = value->rValue;
            here->BJTicVCEGiven = TRUE;
            break;
        case BJT_AREA_SENS:
            here->BJTsenParmNo = value->iValue;
            break;
        case BJT_IC :
            switch(value->v.numValue) {
                case 2:
                    here->BJTicVCE = *(value->v.vec.rVec+1);
                    here->BJTicVCEGiven = TRUE;
                case 1:
                    here->BJTicVBE = *(value->v.vec.rVec);
                    here->BJTicVBEGiven = TRUE;
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
