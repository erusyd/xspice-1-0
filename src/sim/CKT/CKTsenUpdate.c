/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* CKTsenUpdate(ckt)
 * this is a driver program to iterate through all the various
 * sensitivity update functions provided for the circuit elements 
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

RCSID("CKTsenUpdate.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:19 $")

int
CKTsenUpdate(ckt)
register CKTcircuit *ckt;
{
    extern SPICEdev *DEVices[];
    register int i;
    int error;


    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVsenUpdate != NULL) 
            && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVsenUpdate))(ckt->CKThead[i],ckt);
            if(error) return(error);
        }
    }
    return(OK);
}
