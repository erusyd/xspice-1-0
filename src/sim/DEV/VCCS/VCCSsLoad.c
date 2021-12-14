/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* actually load the current sensitivity information into the 
 * array  previously provided 
 */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "VCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCCSsLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:10 $")

int
VCCSsLoad(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    register VCCSmodel *model = (VCCSmodel *)inModel;
    register VCCSinstance *here;
    double  vc;

    /*  loop through all the source models */
    for( ; model != NULL; model = model->VCCSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCCSinstances; here != NULL ;
                here=here->VCCSnextInstance) {
            if(here->VCCSsenParmNo){

                vc = *(ckt->CKTrhsOld + here->VCCScontPosNode)
                        -  *(ckt->CKTrhsOld + here->VCCScontNegNode);
                *(ckt->CKTsenInfo->SEN_RHS[here->VCCSposNode] + 
                        here->VCCSsenParmNo) -= vc;
                *(ckt->CKTsenInfo->SEN_RHS[here->VCCSnegNode] + 
                        here->VCCSsenParmNo) += vc;
            }
        }
    }
    return(OK);
}


