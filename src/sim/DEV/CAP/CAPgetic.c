/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CKTdefs.h"
#include "CAPdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CAPgetic.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:26 $")

int
CAPgetic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;

{

    CAPmodel *model = (CAPmodel*)inModel;
    CAPinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->CAPnextModel) {
        for(here = model->CAPinstances; here ; here = here->CAPnextInstance) {
            if(!here->CAPicGiven) {
                here->CAPinitCond = 
                        *(ckt->CKTrhs + here->CAPposNode) - 
                        *(ckt->CKTrhs + here->CAPnegNode);
            }
        }
    }
    return(OK);
}

