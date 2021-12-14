/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* Pretty print the sensitivity info for all 
 * the CCCS in the circuit.
 */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "CCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCCSsPrint.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:52 $")

void
CCCSsPrint(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register CCCSmodel *model = (CCCSmodel*)inModel;
    register CCCSinstance *here;

    printf("CURRENT CONTROLLED CURRENT SOURCES-----------------\n");
    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCCSnextModel ) {

        printf("Model name:%s\n",model->CCCSmodName);

        /* loop through all the instances of the model */
        for (here = model->CCCSinstances; here != NULL ;
                here=here->CCCSnextInstance) {

            printf("    Instance name:%s\n",here->CCCSname);
            printf("      Positive, negative nodes: %s, %s\n",
                    CKTnodName(ckt,here->CCCSposNode),
                    CKTnodName(ckt,here->CCCSnegNode));
            printf("      Controlling source name: %s\n",
                    here->CCCScontName);
            printf("      Controlling Branch equation number: %s\n",
                    CKTnodName(ckt,here->CCCScontBranch));
            printf("      Coefficient: %f\n",here->CCCScoeff);
            printf("    CCCSsenParmNo:%d\n",here->CCCSsenParmNo);

        }
    } 
}
