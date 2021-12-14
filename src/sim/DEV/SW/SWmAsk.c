/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

/*
 * This routine gives access to the internal model parameter
 * of voltage controlled SWitch
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "SWdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SWmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:22 $")

/* ARGSUSED */
int
SWmAsk(ckt,inModel,which,value)
    CKTcircuit *ckt;
    GENmodel *inModel;
    int which;
    IFvalue *value;
{
    SWmodel *model = (SWmodel *)inModel;
    switch(which) {
        case SW_MOD_RON:
            value->rValue = model->SWonResistance;
            return (OK);
        case SW_MOD_ROFF:
            value->rValue = model->SWoffResistance;
            return (OK);
        case SW_MOD_VTH:
            value->rValue = model->SWvThreshold;
            return (OK);
        case SW_MOD_VHYS:
            value->rValue = model->SWvHysteresis;
            return (OK);
        case SW_MOD_GON:
            value->rValue = model->SWonConduct;
            return (OK);
        case SW_MOD_GOFF:
            value->rValue = model->SWoffConduct;
            return (OK);
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
