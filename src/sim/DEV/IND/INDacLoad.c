/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("INDacLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:58 $")

int
INDacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register INDmodel *model = (INDmodel*)inModel;
    double val;
    register INDinstance *here;

    for( ; model != NULL; model = model->INDnextModel) {
        for( here = model->INDinstances;here != NULL; 
                here = here->INDnextInstance) {
    
            val = ckt->CKTomega * here->INDinduct;
            *(here->INDposIbrptr) += 1;
            *(here->INDnegIbrptr) -= 1;
            *(here->INDibrPosptr) += 1;
            *(here->INDibrNegptr) -= 1;
            *(here->INDibrIbrptr +1) -= val;
        }
    }
    return(OK);

}
