/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SMPdefs.h"
#include "RESdefs.h"
#include "SPerror.h"
#include "suffix.h"
#include "CKTdefs.h"

RCSID("RESsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:02 $")

int
RESsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
        /* loop through all the devices and 
         * assign parameter #s to design parameters 
         */
{
    register RESmodel *model = (RESmodel *)inModel;
    register RESinstance *here;

    /*  loop through all the resistor models */
    for( ; model != NULL; model = model->RESnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL ;
            here=here->RESnextInstance) {

            if(here->RESsenParmNo){
                here->RESsenParmNo = ++(info->SENparms);
            }
        }
    }
    return(OK);
}
