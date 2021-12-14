/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* loop through all the devices and 
 * allocate parameter #s to design parameters 
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CCVSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCVSSsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:09 $")

int
CCVSsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;

{
    register CCVSmodel *model = (CCVSmodel*)inModel;
    register CCVSinstance *here;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CCVSinstances; here != NULL ;
                here=here->CCVSnextInstance) {

            if(here->CCVSsenParmNo){
                here->CCVSsenParmNo = ++(info->SENparms);
            }
        }
    }
    return(OK);
}

