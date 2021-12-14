/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* Pretty print the sensitivity info for 
 * all the VCCS in the circuit.
 */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "VCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCCSsPrint.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:10 $")

void
VCCSsPrint(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register VCCSmodel *model = (VCCSmodel *)inModel;
    register VCCSinstance *here;

    printf("VOLTAGE CONTROLLED CURRENT SOURCES-----------------\n");
    /*  loop through all the source models */
    for( ; model != NULL; model = model->VCCSnextModel ) {

        printf("Model name:%s\n",model->VCCSmodName);

        /* loop through all the instances of the model */
        for (here = model->VCCSinstances; here != NULL ;
                here=here->VCCSnextInstance) {

            printf("    Instance name:%s\n",here->VCCSname);
            printf("      Positive, negative nodes: %s, %s\n",
            CKTnodName(ckt,here->VCCSposNode),
                    CKTnodName(ckt,here->VCCSnegNode));
            printf("      Controlling Positive, negative nodes: %s, %s\n",
            CKTnodName(ckt,here->VCCScontPosNode),
                    CKTnodName(ckt,here->VCCScontNegNode));
            printf("      Coefficient: %f\n",here->VCCScoeff);
            printf("    VCCSsenParmNo:%d\n",here->VCCSsenParmNo);
        }
    }
}
