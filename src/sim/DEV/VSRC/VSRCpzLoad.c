/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "VSRCdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("VSRCpzLoad.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:51 $")

/* ARGSUSED */
int
VSRCpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;
{
    register VSRCmodel *model = (VSRCmodel *)inModel;
    register VSRCinstance *here;

    for( ; model != NULL; model = model->VSRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VSRCinstances; here != NULL ;
                here=here->VSRCnextInstance) {

            if (!(here->VSRCacGiven)) {
                /*a dc source*/
                /*the connecting nodes are shorted*/
                *(here->VSRCposIbrptr)  += 1.0 ;
                *(here->VSRCnegIbrptr)  += -1.0 ;
                *(here->VSRCibrPosptr)  += 1.0 ;
                *(here->VSRCibrNegptr)  += -1.0 ;
            } else {
                /*an ac source*/
                /*no effective contribution
                 *diagonal element made 1
                 */
                *(here->VSRCposIbrptr)  += 1.0 ;
                *(here->VSRCnegIbrptr)  += -1.0 ;
                *(here->VSRCibrIbrptr)  += 1.0 ;
            }
        }
    }
    return(OK);
}
