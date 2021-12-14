/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "JFETdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("JFETtrunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:52 $")

int
JFETtrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register JFETmodel *model = (JFETmodel*)inModel;
    register JFETinstance *here;

    for( ; model != NULL; model = model->JFETnextModel) {
        for(here=model->JFETinstances;here!=NULL;here = here->JFETnextInstance){
            CKTterr(here->JFETqgs,ckt,timeStep);
            CKTterr(here->JFETqgd,ckt,timeStep);
        }
    }
    return(OK);
}
