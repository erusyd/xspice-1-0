/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* Pretty print the sensitivity info for all 
 * the CCVS in the circuit.
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CCVSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCVSsPrint.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:09 $")

void
CCVSsPrint(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register CCVSmodel *model = (CCVSmodel*)inModel;
    register CCVSinstance *here;

    printf("CURRENT CONTROLLED VOLTAGE SOURCES-----------------\n");
    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCVSnextModel ) {

        printf("Model name:%s\n",model->CCVSmodName);

        /* loop through all the instances of the model */
        for (here = model->CCVSinstances; here != NULL ;
                here=here->CCVSnextInstance) {
            printf("    Instance name:%s\n",here->CCVSname);
            printf("      Positive, negative nodes: %s, %s\n",
                    CKTnodName(ckt,here->CCVSposNode),
                    CKTnodName(ckt,here->CCVSnegNode));
            printf("      Controlling source name: %s\n",
                    here->CCVScontName);
            printf("      Branch equation number: %s\n",
                    CKTnodName(ckt,here->CCVSbranch));
            printf("      Controlling Branch equation number: %s\n",
                    CKTnodName(ckt,here->CCVScontBranch));
            printf("      Coefficient: %f\n",here->CCVScoeff);
            printf("    CCVSsenParmNo:%d\n",here->CCVSsenParmNo);
        }
    } 
}

