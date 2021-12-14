/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("INDtrunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:09 $")

int
INDtrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    register double *timeStep;
{
    register INDmodel *model = (INDmodel*)inModel;
    register INDinstance *here;
    for( ; model!= NULL; model = model->INDnextModel) {
        for(here = model->INDinstances ; here != NULL ;
                here = here->INDnextInstance) {
            CKTterr(here->INDflux,ckt,timeStep);
        }
    }
    return(OK);
}
