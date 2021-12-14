/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTtemp(ckt)
     * this is a driver program to iterate through all the various
     * temperature dependency functions provided for the circuit 
     * elements in the given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "CONST.h"
#include <math.h>
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTtemp.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:26 $")

extern SPICEdev *DEVices[];

int
CKTtemp(ckt)
    register CKTcircuit *ckt;

{
    int error;
    register int i;

    ckt->CKTvt = CONSTKoverQ * ckt->CKTtemp;

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVtemperature != NULL) &&
                (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVtemperature))(ckt->CKThead[i],ckt);
            if(error) return(error);
        }
    }
    return(OK);
}
