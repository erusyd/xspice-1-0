/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* actually load the current sensitivity information
 * into the array previously provided 
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CCVSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCVSsSetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:08 $")

int
CCVSsLoad(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    register CCVSmodel *model = (CCVSmodel*)inModel;
    register CCVSinstance *here;
    double   ic;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CCVSinstances; here != NULL ;
                here=here->CCVSnextInstance) {
            if(here->CCVSsenParmNo){
                ic = *(ckt->CKTrhsOld + here->CCVScontBranch);
                *(ckt->CKTsenInfo->SEN_RHS[here->CCVSbranch] + 
                        here->CCVSsenParmNo) -= ic;
            }
        }
    }
    return(OK);
}

