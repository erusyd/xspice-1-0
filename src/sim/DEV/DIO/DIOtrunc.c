/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOtrunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:49 $")

int
DIOtrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register DIOmodel *model = (DIOmodel*)inModel;
    register DIOinstance *here;

    for( ; model != NULL; model = model->DIOnextModel) {
        for(here=model->DIOinstances;here!=NULL;here = here->DIOnextInstance){
            CKTterr(here->DIOcapCharge,ckt,timeStep);
        }
    }
    return(OK);
}
