/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "VCVSdefs.h"
#include "suffix.h"

RCSID("VCVSpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:28 $")

/*ARGSUSED*/
int
VCVSpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;
        /* actually load the current voltage value into the 
         * sparse matrix previously provided 
         */
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCVSinstances; here != NULL ;
                here=here->VCVSnextInstance) {
            
            *(here->VCVSposIbrptr) += 1.0 ;
            *(here->VCVSnegIbrptr) -= 1.0 ;
            *(here->VCVSibrPosptr) += 1.0 ;
            *(here->VCVSibrNegptr) -= 1.0 ;
            *(here->VCVSibrContPosptr) -= here->VCVScoeff ;
            *(here->VCVSibrContNegptr) += here->VCVScoeff ;
        }
    }
    return(OK);
}
