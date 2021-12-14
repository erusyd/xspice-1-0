/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "MOS3defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS3getic.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:25 $")

int
MOS3getic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    MOS3model *model = (MOS3model *)inModel;
    MOS3instance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->MOS3nextModel) {
        for(here = model->MOS3instances; here ; here = here->MOS3nextInstance) {
            if(!here->MOS3icVBSGiven) {
                here->MOS3icVBS = 
                        *(ckt->CKTrhs + here->MOS3bNode) - 
                        *(ckt->CKTrhs + here->MOS3sNode);
            }
            if(!here->MOS3icVDSGiven) {
                here->MOS3icVDS = 
                        *(ckt->CKTrhs + here->MOS3dNode) - 
                        *(ckt->CKTrhs + here->MOS3sNode);
            }
            if(!here->MOS3icVGSGiven) {
                here->MOS3icVGS = 
                        *(ckt->CKTrhs + here->MOS3gNode) - 
                        *(ckt->CKTrhs + here->MOS3sNode);
            }
        }
    }
    return(OK);
}
