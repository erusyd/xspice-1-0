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
#include "CKTdefs.h"
#include "SMPdefs.h"
#include "BJTdefs.h"
#include "CONST.h"
#include "SPerror.h"
#include "IFsim.h"
#include "suffix.h"

RCSID("BJTsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:49 $")

int
BJTsSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register BJTmodel *model = (BJTmodel*)inModel;
    register BJTinstance *here;

#ifdef STEPDEBUG
    printf(" BJTsensetup \n");
#endif /* STEPDEBUG */

    /*  loop through all the diode models */
    for( ; model != NULL; model = model->BJTnextModel ) {


        /* loop through all the instances of the model */
        for (here = model->BJTinstances; here != NULL ;
                here=here->BJTnextInstance) {

            if(here->BJTsenParmNo){
                here->BJTsenParmNo = ++(info->SENparms);
                here->BJTsenPertFlag = OFF;
            }
            if((here->BJTsens = (double *)MALLOC(55*sizeof(double))) ==
                NULL) return(E_NOMEM);
        }
    }
    return(OK);
}

