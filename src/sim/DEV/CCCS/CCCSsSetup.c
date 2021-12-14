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
#include "CCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCCSsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:52 $")

int
CCCSsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register CCCSmodel *model = (CCCSmodel*)inModel;
    register CCCSinstance *here;

    /*  loop through all the CCCS models */
    for( ; model != NULL; model = model->CCCSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CCCSinstances; here != NULL ;
                here=here->CCCSnextInstance) {

            if(here->CCCSsenParmNo){
                here->CCCSsenParmNo = ++(info->SENparms);
            }

        }
    }
    return(OK);
}

