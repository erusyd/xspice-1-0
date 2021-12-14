/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "IFsim.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MUTparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:13 $")

#ifdef MUTUAL
/* ARGSUSED */
int
MUTparam(param,value,inst,select)
    int param;
    IFvalue *value;
    GENinstance *inst;
    IFvalue *select;
{
    MUTinstance *here = (MUTinstance*)inst;
    switch(param) {
        case MUT_COEFF:
            here->MUTcoupling = value->rValue;
            here->MUTindGiven = TRUE;
            break;
        case MUT_IND1:
            here->MUTindName1 = value->uValue;
            break;
        case MUT_IND2:
            here->MUTindName2 = value->uValue;
            break;
        case MUT_COEFF_SENS:
        here->MUTsenParmNo = value->iValue;
        break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
#endif /* MUTUAL */
