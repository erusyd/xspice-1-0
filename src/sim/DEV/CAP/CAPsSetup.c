/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* loop through all the devices and 
 * allocate parameter #s to design parameters 
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CKTdefs.h"
#include "CAPdefs.h"
#include "TRANdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CAPsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:34 $")

int
CAPsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;

{
    register CAPmodel *model = (CAPmodel*)inModel;
    register CAPinstance *here;

    /*  loop through all the capacitor models */
    for( ; model != NULL; model = model->CAPnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CAPinstances; here != NULL ;
                here=here->CAPnextInstance) {


            if(here->CAPsenParmNo){
                here->CAPsenParmNo = ++(info->SENparms);
            }
        }
    }
    return(OK);
}

