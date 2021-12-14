/*
 * Copyright (c) 1987 Thomas L. Quarles
 */


/*
 * This routine gives access to the internal device parameters
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

RCSID("URCask.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:51 $")

/* ARGSUSED */
int
URCask(ckt,inst,which,value,select)
    CKTcircuit *ckt;
    GENinstance *inst;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    URCinstance *here = (URCinstance *)inst;
    switch(which) {
        case URC_POS_NODE:
            value->iValue = here->URCposNode;
            return (OK);
        case URC_NEG_NODE:
            value->iValue = here->URCnegNode;
            return (OK);
        case URC_GND_NODE:
            value->iValue = here->URCgndNode;
            return (OK);
        case URC_LEN:
            value->rValue = here->URClength;
            return (OK);
        case URC_LUMPS:
            value->iValue = here->URClumps;
            return (OK);
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
