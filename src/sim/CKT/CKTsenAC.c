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

RCSID("CKTsenAc.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:10 $")

/* CKTsenAC(ckt)
 * this is a routine for AC sensitivity calculations
 */

int
CKTsenAC(ckt)
register CKTcircuit *ckt;
{
    int error;

#ifdef SENSDEBUG
    printf("CKTsenAC\n");
#endif /* SENSDEBUG */


    if(error = CKTsenLoad(ckt)) return(error);

#ifdef SENSDEBUG
    printf("after CKTsenLoad\n");
#endif /* SENSDEBUG */

    if(error = CKTsenComp(ckt)) return(error);

#ifdef SENSDEBUG
    printf("after CKTsenComp\n");
#endif /* SENSDEBUG */

    return(OK);
}

