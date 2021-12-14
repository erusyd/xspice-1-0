/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "BSIMdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMgetic.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:05 $")

int
BSIMgetic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;

{

    BSIMmodel *model = (BSIMmodel*)inModel;
    BSIMinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->BSIMnextModel) {
        for(here = model->BSIMinstances; here ; here = here->BSIMnextInstance) {
            if(!here->BSIMicVBSGiven) {
                here->BSIMicVBS = 
                        *(ckt->CKTrhs + here->BSIMbNode) - 
                        *(ckt->CKTrhs + here->BSIMsNode);
            }
            if(!here->BSIMicVDSGiven) {
                here->BSIMicVDS = 
                        *(ckt->CKTrhs + here->BSIMdNode) - 
                        *(ckt->CKTrhs + here->BSIMsNode);
            }
            if(!here->BSIMicVGSGiven) {
                here->BSIMicVGS = 
                        *(ckt->CKTrhs + here->BSIMgNode) - 
                        *(ckt->CKTrhs + here->BSIMsNode);
            }
        }
    }
    return(OK);
}
