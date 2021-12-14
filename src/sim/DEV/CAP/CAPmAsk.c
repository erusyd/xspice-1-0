/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "CAPdefs.h"
#include "SPerror.h"
#include "IFsim.h"
#include "suffix.h"

RCSID("CAPmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:28 $")

/* ARGSUSED */
int
CAPmAsk(ckt,inst,which,value)
    CKTcircuit *ckt;
    GENmodel *inst;
    int which;
    IFvalue *value;
{
    CAPmodel *here = (CAPmodel*)inst;
    switch(which) {
        case CAP_MOD_CJ:
            value->rValue = here->CAPcj;
            return(OK);
        case CAP_MOD_CJSW:
            value->rValue = here->CAPcjsw;
            return(OK);
        case CAP_MOD_DEFWIDTH:
            value->rValue = here->CAPdefWidth;
            return(OK);
        case CAP_MOD_NARROW:
            value->rValue = here->CAPnarrow;
            return(OK);
        default:  
            return(E_BADPARM);
    }
}
