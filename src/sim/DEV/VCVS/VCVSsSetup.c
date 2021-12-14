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
#include "VCVSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:31 $")

int
VCVSsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCVSinstances; here != NULL ;
                here=here->VCVSnextInstance) {

            if(here->VCVSsenParmNo){
                here->VCVSsenParmNo = ++(info->SENparms);
            }

        }
    }
    return(OK);
}

