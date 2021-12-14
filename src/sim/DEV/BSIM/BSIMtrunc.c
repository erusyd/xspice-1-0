/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "BSIMdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMtrunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:14 $")

int
BSIMtrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;

{
    register BSIMmodel *model = (BSIMmodel*)inModel;
    register BSIMinstance *here;
#ifdef STEPDEBUG
    double debugtemp;
#endif /* STEPDEBUG */

    for( ; model != NULL; model = model->BSIMnextModel) {
        for(here=model->BSIMinstances;here!=NULL;here = here->BSIMnextInstance){
#ifdef STEPDEBUG
            debugtemp = *timeStep;
#endif /* STEPDEBUG */
            CKTterr(here->BSIMqb,ckt,timeStep);
            CKTterr(here->BSIMqg,ckt,timeStep);
            CKTterr(here->BSIMqd,ckt,timeStep);
#ifdef STEPDEBUG
            if(debugtemp != *timeStep) {
                printf("device %s reduces step from %g to %g\n",
                        here->BSIMname,debugtemp,*timeStep);
            }
#endif /* STEPDEBUG */
        }
    }
    return(OK);
}

