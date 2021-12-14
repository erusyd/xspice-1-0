/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* loop through all the devices and 
 * allocate parameter #s to design parameters 
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:46 $")

int
DIOsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register DIOmodel *model = (DIOmodel*)inModel;
    register DIOinstance *here;

    /*  loop through all the diode models */
    for( ; model != NULL; model = model->DIOnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->DIOinstances; here != NULL ;
                here=here->DIOnextInstance) {

            if(here->DIOsenParmNo){
                here->DIOsenParmNo = ++(info->SENparms);
                here->DIOsenPertFlag = OFF;
            }
            if((here->DIOsens = (double *)MALLOC(7*sizeof(double)))
                    == NULL) return(E_NOMEM);

        }
    }
    return(OK);
}

