/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "VSRCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VSRCacLoad.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:29 $")

int
VSRCacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register VSRCmodel *model = (VSRCmodel *)inModel;
    register VSRCinstance *here;

    for( ; model != NULL; model = model->VSRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VSRCinstances; here != NULL ;
                here=here->VSRCnextInstance) {

            *(here->VSRCposIbrptr) += 1.0 ;
            *(here->VSRCnegIbrptr) -= 1.0 ;
            *(here->VSRCibrPosptr) += 1.0 ;
            *(here->VSRCibrNegptr) -= 1.0 ;
            *(ckt->CKTrhs + (here->VSRCbranch)) += here->VSRCacReal;
            *(ckt->CKTirhs + (here->VSRCbranch)) += here->VSRCacImag;
        }
    }
    return(OK);
}
