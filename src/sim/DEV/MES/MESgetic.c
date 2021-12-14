/*
 * Copyright (c) 1985 S. Hwang
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CKTdefs.h"
#include "MESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MESgetic.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:03 $")

int
MESgetic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    MESmodel *model = (MESmodel*)inModel;
    MESinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->MESnextModel) {
        for(here = model->MESinstances; here ; here = here->MESnextInstance) {
            if(!here->MESicVDSGiven) {
                here->MESicVDS = 
                        *(ckt->CKTrhs + here->MESdrainNode) - 
                        *(ckt->CKTrhs + here->MESsourceNode);
            }
            if(!here->MESicVGSGiven) {
                here->MESicVGS = 
                        *(ckt->CKTrhs + here->MESgateNode) - 
                        *(ckt->CKTrhs + here->MESsourceNode);
            }
        }
    }
    return(OK);
}
