/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CAPdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CAPacitor.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:37 $")

int
CAPtrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    register double *timeStep;
{
    register CAPmodel *model = (CAPmodel*)inModel;
    register CAPinstance *here;

    for( ; model!= NULL; model = model->CAPnextModel) {
        for(here = model->CAPinstances ; here != NULL ;
                here = here->CAPnextInstance) {
            CKTterr(here->CAPqcap,ckt,timeStep);
        }
    }
    return(OK);
}
