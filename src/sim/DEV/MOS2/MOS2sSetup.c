/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "MOS2defs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS2sSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:03 $")

int
MOS2sSetup(info,inModel)
register SENstruct *info;
GENmodel *inModel;
/* loop through all the devices and 
         * allocate parameter #s to design parameters 
         */
{
    register MOS2model *model = (MOS2model *)inModel;
    register MOS2instance *here;

    /*  loop through all the models */
    for( ; model != NULL; model = model->MOS2nextModel ) {

        /* loop through all the instances of the model */
        for (here = model->MOS2instances; here != NULL ;
                here=here->MOS2nextInstance) {


            if(here->MOS2senParmNo){
                if((here->MOS2sens_l)&&(here->MOS2sens_w)){
                    here->MOS2senParmNo = ++(info->SENparms);
                    ++(info->SENparms);/* MOS has two design parameters */
                }
                else{
                    here->MOS2senParmNo = ++(info->SENparms);
                }
            }
            here->MOS2senPertFlag = OFF;
            if((here->MOS2sens = (double *)MALLOC(70*sizeof(double))) == NULL) {
                return(E_NOMEM);
            }

        }
    }
    return(OK);
}


