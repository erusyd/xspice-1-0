/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CAPdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CAPacLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:23 $")

int
CAPacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;

{
    register CAPmodel *model = (CAPmodel*)inModel;
    double val;
    register CAPinstance *here;

    for( ; model != NULL; model = model->CAPnextModel) {
        for( here = model->CAPinstances;here != NULL; 
                here = here->CAPnextInstance) {
    
            val = ckt->CKTomega * here->CAPcapac;
            *(here->CAPposPosptr +1) += val;
            *(here->CAPnegNegptr +1) += val;
            *(here->CAPposNegptr +1) -= val;
            *(here->CAPnegPosptr +1) -= val;
        }
    }
    return(OK);

}

