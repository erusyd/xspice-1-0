/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "CCCSdefs.h"
#include "suffix.h"

RCSID("CCCSpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:50 $")

/*ARGSUSED*/
int
CCCSpzLoad(inModel,ckt,s)

    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;

        /* actually load the current voltage value into the 
         * sparse matrix previously provided 
         */
{
    register CCCSmodel *model = (CCCSmodel*)inModel;
    register CCCSinstance *here;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCCSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CCCSinstances; here != NULL ;
                here=here->CCCSnextInstance) {
            
            *(here->CCCSposContBrptr) += here->CCCScoeff ;
            *(here->CCCSnegContBrptr) -= here->CCCScoeff ;
        }
    }
    return(OK);
}
