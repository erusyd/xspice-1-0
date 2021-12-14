/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "RESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("RESsAcLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:59 $")

int
RESsAcLoad(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /* actually load the current ac sensitivity info into the 
         * array previously provided 
         */
{
    register RESmodel *model = (RESmodel *)inModel;
    register RESinstance *here;
    double vres;
    double ivres;
    double value;
    double ivalue;

    /*  loop through all the resistor models */
    for( ; model != NULL; model = model->RESnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL ;
                here=here->RESnextInstance) {

            if(here->RESsenParmNo){
                vres = *(ckt->CKTrhsOld+here->RESposNode) -
                    *(ckt->CKTrhsOld+here->RESnegNode);
                ivres = *(ckt->CKTirhsOld+here->RESposNode) -
                    *(ckt->CKTirhsOld+here->RESnegNode);
                value = vres * here->RESconduct * here->RESconduct;
                ivalue = ivres * here->RESconduct * here->RESconduct;

                /* load the RHS matrix */
                *(ckt->CKTsenInfo->SEN_RHS[here->RESposNode] + 
                        here->RESsenParmNo) += value;
                *(ckt->CKTsenInfo->SEN_iRHS[here->RESposNode] +
                        here->RESsenParmNo) += ivalue;
                *(ckt->CKTsenInfo->SEN_RHS[here->RESnegNode] + 
                        here->RESsenParmNo) -= value;
                *(ckt->CKTsenInfo->SEN_iRHS[here->RESnegNode] + 
                        here->RESsenParmNo) -= ivalue;
            }
        }
    }
    return(OK);
}


