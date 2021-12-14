/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCVSload.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:04 $")

/*ARGSUSED*/
int
CCVSload(inModel,ckt)

    GENmodel *inModel;
    CKTcircuit *ckt;

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
            *(here->CCVSibrContBrptr) -= here->CCVScoeff ;
        }
    }
    return(OK);
}
