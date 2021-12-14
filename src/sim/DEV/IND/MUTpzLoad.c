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

RCSID("MUTpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:14 $")

#ifdef MUTUAL
/* ARGSUSED */
int
MUTpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    CKTcircuit *ckt;
    register SPcomplex *s;
{
    register MUTmodel *model = (MUTmodel*)inModel;
    double val;
    register MUTinstance *here;

    for( ; model != NULL; model = model->MUTnextModel) {
        for( here = model->MUTinstances;here != NULL; 
                here = here->MUTnextInstance) {
    
            val =  here->MUTfactor;
            *(here->MUTbr1br2 ) -= val * s->real;
            *(here->MUTbr1br2 +1) -= val * s->imag;
            *(here->MUTbr2br1 ) -= val * s->real;
            *(here->MUTbr2br1 +1) -= val * s->imag;
        }
    }
    return(OK);

}
#endif /*MUTUAL*/
