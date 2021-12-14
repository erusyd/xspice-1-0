/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /*
     * NIinit(nistruct,loadpkg)
     *
     *  Initialize the Numerical iteration package to perform Newton-Raphson
     *  iterations on a sparse matrix filled by the specified load package,
     */

#include "prefix.h"
#include "CKTdefs.h"
#include <stdio.h>
#include "util.h"
#include "SPerror.h"
#include "SMPdefs.h"
#include "suffix.h"

RCSID("NIinit.c $Revision: 1.1 $ on $Date: 91/04/02 11:47:25 $ ")

int
NIinit(ckt)
    CKTcircuit  *ckt;
{
#ifdef SPARSE
/* a concession to Ken Kundert's sparse matrix package - SMP doesn't need this*/
    int Error;
#endif /* SPARSE */
    ckt->CKTniState = NIUNINITIALIZED;
    return(SMPnewMatrix( &(ckt->CKTmatrix) ) );
}
