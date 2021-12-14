/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "ISRCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("ISRCacLoad.c $Revision: 1.1 $ on $Date: 91/07/11 11:06:19 $")

int
ISRCacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register ISRCmodel *model = (ISRCmodel*)inModel;
    register ISRCinstance *here;

    for( ; model != NULL; model = model->ISRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->ISRCinstances; here != NULL ;
                here=here->ISRCnextInstance) {

            *(ckt->CKTrhs + (here->ISRCposNode)) += 
                here->ISRCacReal;
            *(ckt->CKTrhs + (here->ISRCnegNode)) -= 
                here->ISRCacReal;
            *(ckt->CKTirhs + (here->ISRCposNode)) += 
                here->ISRCacImag;
            *(ckt->CKTirhs + (here->ISRCnegNode)) -= 
                here->ISRCacImag;
        }
    }
    return(OK);

}
