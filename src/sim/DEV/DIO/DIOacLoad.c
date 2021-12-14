/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "util.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOacLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:32 $")

int
DIOacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;

{
    register DIOmodel *model = (DIOmodel*)inModel;
    double gspr;
    double geq;
    double xceq;
    register DIOinstance *here;

    /*  loop through all the diode models */
    for( ; model != NULL; model = model->DIOnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->DIOinstances; here != NULL ;
                here=here->DIOnextInstance) {

            gspr=model->DIOconductance*here->DIOarea;
            geq= *(ckt->CKTstate0 + here->DIOconduct);
            xceq= *(ckt->CKTstate0 + here->DIOcapCurrent) * ckt->CKTomega;
            *(here->DIOposPosPtr ) += gspr;
            *(here->DIOnegNegPtr ) += geq;
            *(here->DIOnegNegPtr +1 ) += xceq;
            *(here->DIOposPrimePosPrimePtr ) += geq+gspr;
            *(here->DIOposPrimePosPrimePtr +1 ) += xceq;
            *(here->DIOposPosPrimePtr ) -= gspr;
            *(here->DIOnegPosPrimePtr ) -= geq;
            *(here->DIOnegPosPrimePtr +1 ) -= xceq;
            *(here->DIOposPrimePosPtr ) -= gspr;
            *(here->DIOposPrimeNegPtr ) -= geq;
            *(here->DIOposPrimeNegPtr +1 ) -= xceq;
        }
    }
    return(OK);

}
