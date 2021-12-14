/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "INDdefs.h"
#include "suffix.h"

RCSID("INDpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:03 $")

/* ARGSUSED */
int
INDpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;
{
    register INDmodel *model = (INDmodel*)inModel;
    double val;
    register INDinstance *here;

    for( ; model != NULL; model = model->INDnextModel) {
        for( here = model->INDinstances;here != NULL; 
                here = here->INDnextInstance) {
    
            val = here->INDinduct;
            *(here->INDposIbrptr) += 1;
            *(here->INDnegIbrptr) -= 1;
            *(here->INDibrPosptr) += 1;
            *(here->INDibrNegptr) -= 1;
            *(here->INDibrIbrptr ) -= val * s->real;
            *(here->INDibrIbrptr +1) -= val * s->imag;
        }
    }
    return(OK);

}
