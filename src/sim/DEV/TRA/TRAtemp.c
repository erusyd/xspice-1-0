/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "TRAdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("TRAtemp.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:41 $")

/* ARGSUSED */
int
TRAtemp(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /*
         * pre-process parameters for later use
         */
{
    register TRAmodel *model = (TRAmodel *)inModel;
    register TRAinstance *here;

    /*  loop through all the transmission line models */
    for( ; model != NULL; model = model->TRAnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->TRAinstances; here != NULL ;
                here=here->TRAnextInstance) {
            
            if(!here->TRAtdGiven) {
                here->TRAtd = here->TRAnl/here->TRAf;
            }
            here->TRAconduct = 1/here->TRAimped;
        }
    }
    return(OK);
}
