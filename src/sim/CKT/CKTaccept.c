/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTaccept(ckt)
     * this is a driver program to iterate through all the various
     * accept functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTaccept.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:06 $")

int
CKTaccept(ckt)
    register CKTcircuit *ckt;

{
    extern SPICEdev *DEVices[];

    register int i;
    int size;
    int error;
    double *temp;

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVaccept != NULL) && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVaccept))(ckt,ckt->CKThead[i]);
            if(error) return(error);
        }
    }
#ifdef PREDICTOR
    /* now, move the sols vectors around */
    temp = ckt->CKTsols[7];
    for ( i=7;i>0;i--) {
        ckt->CKTsols[i] = ckt->CKTsols[i-1];
    }
    ckt->CKTsols[0]=temp;
    size = SMPmatSize(ckt->CKTmatrix);
    for(i=0;i<=size;i++) {
        ckt->CKTsols[0][i]=ckt->CKTrhs[i];
    }
#endif /* PREDICTOR */
    return(OK);
}
