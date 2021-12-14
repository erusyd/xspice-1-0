/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "MOS1defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS1trunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:40 $")

int
MOS1trunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register MOS1model *model = (MOS1model *)inModel;
    register MOS1instance *here;

    for( ; model != NULL; model = model->MOS1nextModel) {
        for(here=model->MOS1instances;here!=NULL;here = here->MOS1nextInstance){
            CKTterr(here->MOS1qgs,ckt,timeStep);
            CKTterr(here->MOS1qgd,ckt,timeStep);
            CKTterr(here->MOS1qgb,ckt,timeStep);
        }
    }
    return(OK);
}
