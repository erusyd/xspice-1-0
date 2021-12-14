/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "RESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("RESsLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:01 $")

int
RESsLoad(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /* actually load the current resistance sensitivity value into 
         * the array previously provided 
         */
{
    register RESmodel *model = (RESmodel *)inModel;
    register RESinstance *here;
    double vres;
    double value;

    /*  loop through all the resistor models */
    for( ; model != NULL; model = model->RESnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL ;
                here=here->RESnextInstance) {

            if(here->RESsenParmNo){
                vres = *(ckt->CKTrhsOld+here->RESposNode) -
                    *(ckt->CKTrhsOld+here->RESnegNode);
                value = vres * here->RESconduct * here->RESconduct;

                /* load the RHS matrix */
                *(ckt->CKTsenInfo->SEN_RHS[here->RESposNode] + 
                        here->RESsenParmNo) += value;
                *(ckt->CKTsenInfo->SEN_RHS[here->RESnegNode] + 
                        here->RESsenParmNo) -= value;
            }

        }
    }
    return(OK);
}

