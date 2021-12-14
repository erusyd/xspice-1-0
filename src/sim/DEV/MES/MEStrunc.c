/*
 * Copyright (c) 1985 S. Hwang
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "MESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MEStrunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:10 $")

int
MEStrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register MESmodel *model = (MESmodel*)inModel;
    register MESinstance *here;

    for( ; model != NULL; model = model->MESnextModel) {
        for(here=model->MESinstances;here!=NULL;here = here->MESnextInstance){
            CKTterr(here->MESqgs,ckt,timeStep);
            CKTterr(here->MESqgd,ckt,timeStep);
        }
    }
    return(OK);
}
