/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "JFETdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("JFETgetic.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:45 $")

int
JFETgetic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    JFETmodel *model = (JFETmodel*)inModel;
    JFETinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->JFETnextModel) {
        for(here = model->JFETinstances; here ; here = here->JFETnextInstance) {
            if(!here->JFETicVDSGiven) {
                here->JFETicVDS = 
                        *(ckt->CKTrhs + here->JFETdrainNode) - 
                        *(ckt->CKTrhs + here->JFETsourceNode);
            }
            if(!here->JFETicVGSGiven) {
                here->JFETicVGS = 
                        *(ckt->CKTrhs + here->JFETgateNode) - 
                        *(ckt->CKTrhs + here->JFETsourceNode);
            }
        }
    }
    return(OK);
}
