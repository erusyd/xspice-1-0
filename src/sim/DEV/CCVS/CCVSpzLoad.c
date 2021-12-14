/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "CCVSdefs.h"
#include "suffix.h"

RCSID("CCVSpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:06 $")

/*ARGSUSED*/
int
CCVSpzLoad(inModel,ckt,s)

    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;

        /* actually load the current voltage value into the 
         * sparse matrix previously provided 
         */
{
    register CCVSmodel *model = (CCVSmodel*)inModel;
    register CCVSinstance *here;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CCVSinstances; here != NULL ;
                here=here->CCVSnextInstance) {
            
            *(here->CCVSposIbrptr) += 1.0 ;
            *(here->CCVSnegIbrptr) -= 1.0 ;
            *(here->CCVSibrPosptr) += 1.0 ;
            *(here->CCVSibrNegptr) -= 1.0 ;
            *(here->CCVSibrContBrptr) += here->CCVScoeff ;
        }
    }
    return(OK);
}
