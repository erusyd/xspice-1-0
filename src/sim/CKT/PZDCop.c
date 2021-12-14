/*
 * Copyright (c) 1985 Mani Bhushan Srivastava
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "TRANdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("PZDCop.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:42 $")

int
PZDCop(ckt) 
    CKTcircuit *ckt;
{
    int NIiter();
    int CKTload();
    int converged;


    ckt->CKTmode = MODEDCOP | MODEINITJCT;
    converged = NIiter(ckt,ckt->CKTdcMaxIter);
    if(converged != 0) {
        return(converged);
    }
    ckt->CKTmode = MODEDCOP | MODEINITSMSIG;
    converged = CKTload(ckt);
    return(converged);
}
