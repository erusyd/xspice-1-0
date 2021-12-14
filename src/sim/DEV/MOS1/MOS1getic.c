/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "MOS1defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS1getic.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:24 $")

int
MOS1getic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    MOS1model *model = (MOS1model *)inModel;
    MOS1instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->MOS1nextModel) {
        for(here = model->MOS1instances; here ; here = here->MOS1nextInstance) {
            if(!here->MOS1icVBSGiven) {
                here->MOS1icVBS = 
                        *(ckt->CKTrhs + here->MOS1bNode) - 
                        *(ckt->CKTrhs + here->MOS1sNode);
            }
            if(!here->MOS1icVDSGiven) {
                here->MOS1icVDS = 
                        *(ckt->CKTrhs + here->MOS1dNode) - 
                        *(ckt->CKTrhs + here->MOS1sNode);
            }
            if(!here->MOS1icVGSGiven) {
                here->MOS1icVGS = 
                        *(ckt->CKTrhs + here->MOS1gNode) - 
                        *(ckt->CKTrhs + here->MOS1sNode);
            }
        }
    }
    return(OK);
}
