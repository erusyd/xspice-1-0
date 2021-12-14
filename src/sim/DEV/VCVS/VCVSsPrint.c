/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* Pretty print the sensitivity info for 
 * all the VCVS in the circuit.
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "VCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSsPrint.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:30 $")

void
VCVSsPrint(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;

    printf("VOLTAGE CONTROLLED VOLTAGE SOURCES-----------------\n");
    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VCVSnextModel ) {

        printf("Model name:%s\n",model->VCVSmodName);

        /* loop through all the instances of the model */
        for (here = model->VCVSinstances; here != NULL ;
                here=here->VCVSnextInstance) {


            printf("    Instance name:%s\n",here->VCVSname);
            printf("      Positive, negative nodes: %s, %s\n",
                    CKTnodName(ckt,here->VCVSposNode),
                    CKTnodName(ckt,here->VCVSnegNode));
            printf("      Controlling Positive, negative nodes: %s, %s\n",
                    CKTnodName(ckt,here->VCVScontPosNode),
                    CKTnodName(ckt,here->VCVScontNegNode));
            printf("      Branch equation number: %s\n",
                    CKTnodName(ckt,here->VCVSbranch));
            printf("      Coefficient: %f\n",here->VCVScoeff);
            printf("    VCVSsenParmNo:%d\n",here->VCVSsenParmNo);
        }
    }
}
