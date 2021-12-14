/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTpzSetup(ckt)
     * this is a driver program to iterate through all the various
     * pzSetup functions provided for the circuit elements in the
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

RCSID("CKTpzSetup.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:08 $")

#define CKALLOC(var,size,type) \
    if(size && (!(var =(type *)MALLOC((size)*sizeof(type))))){\
            return(E_NOMEM);\
}

int
CKTpzSetup(ckt)
    register CKTcircuit *ckt;

{
    extern SPICEdev *DEVices[];

    register int i;
    int error;

    register SMPmatrix *matrix;
    ckt->CKTnumStates=0;

    matrix = ckt->CKTmatrix;

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVpzSetup != NULL) && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVpzSetup))(matrix,ckt->CKThead[i],ckt,
                    &ckt->CKTnumStates);
            if(error) return(error);
        }
    }
    for(i=0;i<=ckt->CKTmaxOrder+1;i++) {
        CKALLOC(ckt->CKTstates[i],ckt->CKTnumStates,double);
    }
    if(ckt->CKTniState & NIUNINITIALIZED) {
        error = NIreinit(ckt);
        if(error) return(error);
    }

    return(OK);
}
