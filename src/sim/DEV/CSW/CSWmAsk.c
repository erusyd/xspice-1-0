/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

/*
 * This routine gives access to the internal model parameters
 * of Current controlled SWitch
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "CSWdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CSWmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:21 $")

/* ARGSUSED */
int
CSWmAsk(ckt,inst,which,value)
    CKTcircuit *ckt;
    GENmodel *inst;
    int which;
    IFvalue *value;
{
    CSWmodel *here = (CSWmodel*)inst;
    switch(which) {
        case CSW_RON:
            value->rValue = here->CSWonResistance;
            return (OK);
        case CSW_ROFF:
            value->rValue = here->CSWoffResistance;
            return (OK);
        case CSW_ITH:
            value->rValue = here->CSWiThreshold;
            return (OK);
        case CSW_IHYS:
            value->rValue = here->CSWiHysteresis;
            return (OK);
        case CSW_GON:
            value->rValue = here->CSWonConduct;
            return (OK);
        case CSW_GOFF:
            value->rValue = here->CSWoffConduct;
            return (OK);
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
