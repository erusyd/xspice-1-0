/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* NIreinit(ckt)
     *  Perform reinitialization necessary for the numerical iteration
     *  package - the matrix has now been fully accessed once, so we know
     *  how big it is, so allocate RHS vector
     */

#include "prefix.h"
#include "CKTdefs.h"
#include <stdio.h>
#include "util.h"
#include "SMPdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("NIreinit.c $Revision: 1.1 $ on $Date: 91/04/02 11:47:34 $ ")

#define CKALLOC(ptr,size,type) if(( ckt->ptr =\
    (type *) MALLOC((size)*sizeof(type))) == NULL) return(E_NOMEM);

int
NIreinit(ckt)
    register CKTcircuit *ckt;

{
    register int size;
    register int i;

    size = SMPmatSize(ckt->CKTmatrix);
    CKALLOC(CKTrhs,size+1,double);
    CKALLOC(CKTrhsOld,size+1,double);
    CKALLOC(CKTrhsSpare,size+1,double);
    CKALLOC(CKTirhs,size+1,double);
    CKALLOC(CKTirhsOld,size+1,double);
    CKALLOC(CKTirhsSpare,size+1,double);
#ifdef PREDICTOR
    CKALLOC(CKTpred,size+1,double);
    for(i=0;i<8;i++) {
        CKALLOC(CKTsols[i],size+1,double);
    }
#endif /* PREDICTOR */
    ckt->CKTniState = NISHOULDREORDER | NIACSHOULDREORDER | NIPZSHOULDREORDER;
    return(0);
}
