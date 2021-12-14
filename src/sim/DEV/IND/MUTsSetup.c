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
#include "IFsim.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MUTsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:16 $")

#ifdef MUTUAL
/*ARGSUSED*/
int
MUTsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register MUTmodel *model = (MUTmodel*)inModel;
    register MUTinstance *here;

    /*  loop through all the inductor models */
    for( ; model != NULL; model = model->MUTnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->MUTinstances; here != NULL ;
                here=here->MUTnextInstance) {

            if(here->MUTsenParmNo){
                here->MUTsenParmNo = ++(info->SENparms);
            }


        }
    }
    return(OK);
}
#endif /* MUTUAL */

