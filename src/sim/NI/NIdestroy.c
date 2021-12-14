/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* NIdestroy(ckt)
     * delete the data structures allocated for numeric integration.
     */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("NIdestroy.c $Revision: 1.1 $ on $Date: 91/04/02 11:47:24 $ ")

void
NIdestroy(ckt)
    register CKTcircuit *ckt;

{
    SMPdestroy(ckt->CKTmatrix);
    if(ckt->CKTrhs)         FREE(ckt->CKTrhs);
    if(ckt->CKTrhsOld)      FREE(ckt->CKTrhsOld);
    if(ckt->CKTrhsSpare)    FREE(ckt->CKTrhsSpare);
    if(ckt->CKTirhs)        FREE(ckt->CKTirhs);
    if(ckt->CKTirhsOld)     FREE(ckt->CKTirhsOld);
    if(ckt->CKTirhsSpare)   FREE(ckt->CKTirhsSpare);
    if(ckt->CKTsenInfo){
        if(ckt->CKTrhsOp) FREE(((CKTcircuit *)ckt)->CKTrhsOp);
        if(ckt->CKTsenRhs) FREE(((CKTcircuit *)ckt)->CKTsenRhs);
        if(ckt->CKTseniRhs) FREE(((CKTcircuit *)ckt)->CKTseniRhs);
        SENdestroy(ckt->CKTsenInfo);
    }
}
