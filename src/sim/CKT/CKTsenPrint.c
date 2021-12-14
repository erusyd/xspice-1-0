/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "TRANdefs.h"
#include "suffix.h"

RCSID("CKTsenPrint.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:15 $")

    /* CKTsenPrint(ckt)
     * this is a driver program to iterate through all the
     * various sensitivity print functions provided for 
     * the circuit elements in the given circuit 
     */

void
CKTsenPrint(ckt)
register CKTcircuit *ckt;
{
    extern SPICEdev *DEVices[];
    register int i;

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVsenPrint != NULL) && (ckt->CKThead[i] != NULL) ){
            (*((*DEVices[i]).DEVsenPrint))(ckt->CKThead[i],ckt);
        }
    }
}
