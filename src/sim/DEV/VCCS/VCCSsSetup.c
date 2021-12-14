/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* loop through all the devices and 
 * allocate parameter #s to design parameters 
 */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "VCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCCSsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:11 $")

int
VCCSsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register VCCSmodel *model = (VCCSmodel *)inModel;
    register VCCSinstance *here;

    /*  loop through all the current source models */
    for( ; model != NULL; model = model->VCCSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCCSinstances; here != NULL ;
                here=here->VCCSnextInstance) {

            if(here->VCCSsenParmNo){
                here->VCCSsenParmNo = ++(info->SENparms);
            }

        }
    }
    return(OK);
}

