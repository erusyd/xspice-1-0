/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

/*
 * This routine gives access to the internal model parameters
 * of Uniform distributed RC lines
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "URCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("URCmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:52 $")

/* ARGSUSED */
int
URCmAsk(ckt,inst,which,value)
    CKTcircuit *ckt;
    GENmodel *inst;
    int which;
    IFvalue *value;
{
    URCmodel *here = (URCmodel *)inst;
    switch(which) {
        case URC_MOD_K:
            value->rValue = here->URCk;
            return (OK);
        case URC_MOD_FMAX:
            value->rValue = here->URCfmax;
            return (OK);
        case URC_MOD_RPERL:
            value->rValue = here->URCrPerL;
            return (OK);
        case URC_MOD_CPERL:
            value->rValue = here->URCcPerL;
            return (OK);
        case URC_MOD_ISPERL:
            value->rValue = here->URCisPerL;
            return (OK);
        case URC_MOD_RSPERL:
            value->rValue = here->URCrsPerL;
            return (OK);
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
