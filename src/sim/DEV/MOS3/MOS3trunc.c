/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "MOS3defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS3trunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:43 $")

int
MOS3trunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register MOS3model *model = (MOS3model *)inModel;
    register MOS3instance *here;

    for( ; model != NULL; model = model->MOS3nextModel) {
        for(here=model->MOS3instances;here!=NULL;here = here->MOS3nextInstance){
            CKTterr(here->MOS3qgs,ckt,timeStep);
            CKTterr(here->MOS3qgd,ckt,timeStep);
            CKTterr(here->MOS3qgb,ckt,timeStep);
        }
    }
    return(OK);
}
