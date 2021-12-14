/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "VCCSdefs.h"
#include "suffix.h"

RCSID("VCCSpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:08 $")

/*ARGSUSED*/
int
VCCSpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;
        /* actually load the current values into the 
         * sparse matrix previously provided 
         */
{
    register VCCSmodel *model = (VCCSmodel *)inModel;
    register VCCSinstance *here;

    /*  loop through all the source models */
    for( ; model != NULL; model = model->VCCSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCCSinstances; here != NULL ;
                here=here->VCCSnextInstance) {
            
            *(here->VCCSposContPosptr) += here->VCCScoeff ;
            *(here->VCCSposContNegptr) -= here->VCCScoeff ;
            *(here->VCCSnegContPosptr) -= here->VCCScoeff ;
            *(here->VCCSnegContNegptr) += here->VCCScoeff ;
        }
    }
    return(OK);
}
