/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*
 * This routine performs truncation error calculations for
 * BJTs in the circuit.
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "BJTdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BJTtrunc.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:52 $")

int
BJTtrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;

{
    register BJTmodel *model = (BJTmodel*)inModel;
    register BJTinstance *here;

    for( ; model != NULL; model = model->BJTnextModel) {
        for(here=model->BJTinstances;here!=NULL;here = here->BJTnextInstance){
            CKTterr(here->BJTqbe,ckt,timeStep);
            CKTterr(here->BJTqbc,ckt,timeStep);
            CKTterr(here->BJTqcs,ckt,timeStep);
        }
    }
    return(OK);
}
