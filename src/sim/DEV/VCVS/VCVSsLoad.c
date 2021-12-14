/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* actually load the current sensitivity information into the 
 * array previously provided 
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "VCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSsLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:29 $")

int
VCVSsLoad(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;
    double   vc;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCVSinstances; here != NULL ;
                here=here->VCVSnextInstance) {
            if(here->VCVSsenParmNo){

                vc = *(ckt->CKTrhsOld + here->VCVScontPosNode)
                        -  *(ckt->CKTrhsOld + here->VCVScontNegNode);

                *(ckt->CKTsenInfo->SEN_RHS[here->VCVSbranch] + 
                        here->VCVSsenParmNo) += vc;
            }
        }
    }
    return(OK);
}

