/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* loop through all the devices and 
     * allocate parameter #s to design parameters 
     */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "MOS3defs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS3sSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:39 $")

int
MOS3sSetup(info,inModel)
    register SENstruct *info;
    GENmodel *inModel;
{
    register MOS3model *model = (MOS3model *)inModel;
    register MOS3instance *here;

    /*  loop through all the models */
    for( ; model != NULL; model = model->MOS3nextModel ) {

        /* loop through all the instances of the model */
        for (here = model->MOS3instances; here != NULL ;
                here=here->MOS3nextInstance) {


            if(here->MOS3senParmNo){
                if((here->MOS3sens_l)&&(here->MOS3sens_w)){
                    here->MOS3senParmNo = ++(info->SENparms);
                    ++(info->SENparms);/* MOS has two design parameters */
                }
                else{
                    here->MOS3senParmNo = ++(info->SENparms);
                }
            }
            here->MOS3senPertFlag = OFF;
            if((here->MOS3sens = (double *)MALLOC(72*sizeof(double))) == NULL) {
                return(E_NOMEM);
            }

        }
    }
    return(OK);
}


