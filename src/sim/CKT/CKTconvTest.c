/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTconvTest(ckt)
     * this is a driver program to iterate through all the various
     * convTest functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTconvTest.c $Revision: 1.2 $ on $Date: 91/08/01 17:11:02 $")

int
CKTconvTest(ckt)
    register CKTcircuit *ckt;
{
    extern SPICEdev *DEVices[];
    register int i;
    int error;

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVconvTest != NULL) && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVconvTest))(ckt->CKThead[i],ckt);
            if(error) return(error);
        }
        if(ckt->CKTnoncon) {
            /*(printf("convTest: device %s failed\n",
                    (*DEVices[i]).DEVpublic.name);*/

            /* gtri - begin - wbk - Fix bug */

            /* return(OK); */

            /* Must return non-zero if noncon is non-zero. */
            /* Caller uses return value to indicate nonconvergence */
            /* instead of value of noncon */

            /* This bug rendered all DEVconvTest() functions */
            /* ineffective! */

            return(1);

            /* gtri - end - wbk - Fix bug */

        }
    }
    return(OK);
}
