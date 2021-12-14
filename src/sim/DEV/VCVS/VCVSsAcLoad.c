/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* actually load the current ac sensitivity information into the 
 * array previously provided 
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "VCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSsAcLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:28 $")

int
VCVSsAcLoad(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;
    double   vc;
    double   ivc;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCVSinstances; here != NULL ;
                here=here->VCVSnextInstance) {
                if(here->VCVSsenParmNo){

                vc = *(ckt->CKTrhsOld + here->VCVScontPosNode)
                        -  *(ckt->CKTrhsOld + here->VCVScontNegNode);
                ivc = *(ckt->CKTirhsOld + here->VCVScontPosNode)
                        -  *(ckt->CKTirhsOld + here->VCVScontNegNode);

                *(ckt->CKTsenInfo->SEN_RHS[here->VCVSbranch] +
                        here->VCVSsenParmNo) += vc;
                *(ckt->CKTsenInfo->SEN_iRHS[here->VCVSbranch] +
                        here->VCVSsenParmNo) += ivc;
            }
        }
    }
    return(OK);
}

