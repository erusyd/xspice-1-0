/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "MOS2defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS2trunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:08 $")

int
MOS2trunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register MOS2model *model = (MOS2model *)inModel;
    register MOS2instance *here;

    for( ; model != NULL; model = model->MOS2nextModel) {
        for(here=model->MOS2instances;here!=NULL;here = here->MOS2nextInstance){
            CKTterr(here->MOS2qgs,ckt,timeStep);
            CKTterr(here->MOS2qgd,ckt,timeStep);
            CKTterr(here->MOS2qgb,ckt,timeStep);
        }
    }
    return(OK);
}
