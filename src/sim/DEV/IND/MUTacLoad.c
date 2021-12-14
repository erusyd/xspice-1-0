/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CKTdefs.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MUTacLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:09 $")

#ifdef MUTUAL
int
MUTacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register MUTmodel *model = (MUTmodel*)inModel;
    double val;
    register MUTinstance *here;

    for( ; model != NULL; model = model->MUTnextModel) {
        for( here = model->MUTinstances;here != NULL; 
                here = here->MUTnextInstance) {
    
            val = ckt->CKTomega * here->MUTfactor;
            *(here->MUTbr1br2 +1) -= val;
            *(here->MUTbr2br1 +1) -= val;
        }
    }
    return(OK);

}
#endif /* MUTUAL */
