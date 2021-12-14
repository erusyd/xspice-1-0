/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* loop through all the devices and 
     * allocate parameter #s to design parameters 
     */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("INDsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:06 $")

int
INDsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register INDmodel *model = (INDmodel*)inModel;
    register INDinstance *here;

    /*  loop through all the inductor models */
    for( ; model != NULL; model = model->INDnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->INDinstances; here != NULL ;
                here=here->INDnextInstance) {
                if(here->INDsenParmNo){
                here->INDsenParmNo = ++(info->SENparms);
            }
        }
    }
    return(OK);
}
