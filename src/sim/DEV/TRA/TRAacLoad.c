/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "TRAdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("TRAacLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:34 $")

int
TRAacLoad(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /* actually load the current values into the 
         * sparse matrix previously provided 
         */
{
    register TRAmodel *model = (TRAmodel *)inModel;
    register TRAinstance *here;
    double real;
    double imag;

    /*  loop through all the transmission line models */
    for( ; model != NULL; model = model->TRAnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->TRAinstances; here != NULL ;
                here=here->TRAnextInstance) {
            real = cos(-ckt->CKTomega*here->TRAtd);
            imag = sin(-ckt->CKTomega*here->TRAtd);
            
            *(here->TRApos1Pos1Ptr) += here->TRAconduct;
            *(here->TRApos1Int1Ptr) -= here->TRAconduct;
            *(here->TRAneg1Ibr1Ptr) -= 1;
            *(here->TRApos2Pos2Ptr) += here->TRAconduct;
            *(here->TRAneg2Ibr2Ptr) -= 1;
            *(here->TRAint1Pos1Ptr) -= here->TRAconduct;
            *(here->TRAint1Int1Ptr) += here->TRAconduct;
            *(here->TRAint1Ibr1Ptr) += 1;
            *(here->TRAint2Int2Ptr) += here->TRAconduct;
            *(here->TRAint2Ibr2Ptr) += 1;
            *(here->TRAibr1Neg1Ptr) -= 1;
            *(here->TRAibr1Pos2Ptr+0) -= real;
            *(here->TRAibr1Pos2Ptr+1) -= imag;
            *(here->TRAibr1Neg2Ptr+0) += real;
            *(here->TRAibr1Neg2Ptr+1) += imag;
            *(here->TRAibr1Int1Ptr) += 1;
            *(here->TRAibr1Ibr2Ptr+0) -= real * here->TRAimped;
            *(here->TRAibr1Ibr2Ptr+1) -= imag * here->TRAimped;
            *(here->TRAibr2Pos1Ptr+0) -= real;
            *(here->TRAibr2Pos1Ptr+1) -= imag;
            *(here->TRAibr2Neg1Ptr+0) += real;
            *(here->TRAibr2Neg1Ptr+1) += imag;
            *(here->TRAibr2Neg2Ptr) -= 1;
            *(here->TRAibr2Int2Ptr) += 1;
            *(here->TRAibr2Ibr1Ptr+0) -= real * here->TRAimped;
            *(here->TRAibr2Ibr1Ptr+1) -= imag * here->TRAimped;
            *(here->TRApos2Int2Ptr) -= here->TRAconduct;
            *(here->TRAint2Pos2Ptr) -= here->TRAconduct;

        }
    }
    return(OK);
}
