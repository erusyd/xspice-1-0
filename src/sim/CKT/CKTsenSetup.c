/*
 * Copyright (c) 1985 Thomas L. Quarles
 */


/*
 * CKTsenSetup(ckt)
 * this is a driver program to iterate through all the various
 * sensitivity setup functions provided for the circuit elements 
 * in the given circuit 
 */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "TRANdefs.h"
#include "suffix.h"

RCSID("CKTsenSetup.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:18 $")

int
CKTsenSetup(ckt)
register CKTcircuit *ckt;

{
    extern SPICEdev *DEVices[];

    register int i;
    int error;
    register SENstruct *info;
#ifdef SENSDEBUG
    printf("CKTsenSetup\n");
#endif /* SENSDEBUG */
    info = ckt->CKTsenInfo;
    info->SENparms = 0; 

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVsenSetup != NULL)
            && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVsenSetup))(info,ckt->CKThead[i]);
            if(error) return(error);
        }
    }
#ifdef SENSDEBUG
    printf("CKTsenSetup end\n");
#endif /* SENSDEBUG */
    return(OK);
}


