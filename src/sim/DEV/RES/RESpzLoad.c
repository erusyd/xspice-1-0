/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "RESdefs.h"
#include "suffix.h"

RCSID("RESpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:58 $")

/*ARGSUSED*/
int
RESpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;
        /* actually load the current resistance value into the 
         * sparse matrix previously provided 
         */
{
    register RESmodel *model = (RESmodel *)inModel;
    register RESinstance *here;

    /*  loop through all the resistor models */
    for( ; model != NULL; model = model->RESnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL ;
                here=here->RESnextInstance) {
            
            *(here->RESposPosptr) += here->RESconduct;
            *(here->RESnegNegptr) += here->RESconduct;
            *(here->RESposNegptr) -= here->RESconduct;
            *(here->RESnegPosptr) -= here->RESconduct;
        }
    }
    return(OK);
}
