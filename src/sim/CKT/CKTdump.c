/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTdump(ckt)
     * this is a simple program to dump the rhs vector to stdout
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("CKTdump.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:26 $")

void
CKTdump(ckt,ref,plot)
    register CKTcircuit *ckt;
    double ref;
    GENERIC *plot;
{
    IFvalue refData;
    IFvalue valData;

    refData.rValue = ref;
    valData.v.numValue = ckt->CKTmaxEqNum-1;
    valData.v.vec.rVec = ckt->CKTrhsOld+1;
    (*(SPfrontEnd->OUTpData))(plot,&refData,&valData);
}
