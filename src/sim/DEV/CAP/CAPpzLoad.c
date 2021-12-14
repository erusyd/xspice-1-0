/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "CAPdefs.h"
#include "suffix.h"

RCSID("CAPpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:31 $")

/* ARGSUSED */
int
CAPpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    CKTcircuit *ckt;
    register SPcomplex *s;

{
    register CAPmodel *model = (CAPmodel*)inModel;
    double val;
    register CAPinstance *here;

    for( ; model != NULL; model = model->CAPnextModel) {
        for( here = model->CAPinstances;here != NULL; 
                here = here->CAPnextInstance) {
    
            val = here->CAPcapac;
            *(here->CAPposPosptr ) += val * s->real;
            *(here->CAPposPosptr +1) += val * s->imag;
            *(here->CAPnegNegptr ) += val * s->real;
            *(here->CAPnegNegptr +1) += val * s->imag;
            *(here->CAPposNegptr ) -= val * s->real;
            *(here->CAPposNegptr +1) -= val * s->imag;
            *(here->CAPnegPosptr ) -= val * s->real;
            *(here->CAPnegPosptr +1) -= val * s->imag;
        }
    }
    return(OK);

}
