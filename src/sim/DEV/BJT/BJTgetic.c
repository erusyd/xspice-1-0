/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*
 * This routine gets the device initial conditions for the BJTs
 * from the RHS vector
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "BJTdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BJTgetic.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:39 $")

int
BJTgetic(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;

{

    BJTmodel *model = (BJTmodel*)inModel;
    register BJTinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->BJTnextModel) {
        for(here = model->BJTinstances; here ; here = here->BJTnextInstance) {
            if(!here->BJTicVBEGiven) {
                here->BJTicVBE = 
                        *(ckt->CKTrhs + here->BJTbaseNode) - 
                        *(ckt->CKTrhs + here->BJTemitNode);
            }
            if(!here->BJTicVCEGiven) {
                here->BJTicVCE = 
                        *(ckt->CKTrhs + here->BJTcolNode) - 
                        *(ckt->CKTrhs + here->BJTemitNode);
            }
        }
    }
    return(OK);
}
