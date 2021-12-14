/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOgetic.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:37 $")

int
DIOgetic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    DIOmodel *model = (DIOmodel*)inModel;
    DIOinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->DIOnextModel) {
        for(here = model->DIOinstances; here ; here = here->DIOnextInstance) {
            if(!here->DIOinitCondGiven) {
                here->DIOinitCond = 
                        *(ckt->CKTrhs + here->DIOposNode) - 
                        *(ckt->CKTrhs + here->DIOnegNode);
            }
        }
    }
    return(OK);
}
