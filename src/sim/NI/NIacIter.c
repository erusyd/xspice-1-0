/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * NIacIter(ckt)
     *
     *  This subroutine performs the actual numerical iteration.
     *  It uses the sparse matrix stored in the NIstruct by NIinit,
     *  along with the matrix loading program, the load data, the
     *  convergence test function, and the convergence parameters
     * - return value is non-zero for convergence failure 
     */


#include "prefix.h"
#include <stdio.h>
#include "TRANdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "SPerror.h"
/* gtri - begin - wbk - add include files */
#include "IPCtiein.h"
/* gtri - end - wbk - add include files */
#include "suffix.h"

RCSID("NIacIter.c $Revision: 1.3 $ on $Date: 91/12/19 17:00:47 $ ")

int
NIacIter(ckt)
    register CKTcircuit * ckt;
{
    int error;
    int ignore;
    double *temp;

retry:
/* gtri - begin - wbk - if ipc mode, check for pause or stop request */
        if(g_ipc.enabled) {
            ipc_check_pause_stop();
            if(g_ipc.stop_analysis) {
                ckt->CKTnoncon = 1;
                return(E_ITERLIM);
            }
        }
/* gtri - end - wbk - if ipc mode, check for pause or stop request */

    ckt->CKTnoncon=0;

    error = CKTacLoad(ckt);
    if(error) return(error);
    
    if(ckt->CKTniState & NIACSHOULDREORDER) {
        error = SMPcReorder(ckt->CKTmatrix,ckt->CKTpivotAbsTol,
                ckt->CKTpivotRelTol,&ignore);
        ckt->CKTniState &= ~NIACSHOULDREORDER;
        if(error != 0) {
            /* either singular equations or no memory, in either case,
             * let caller handle problem
             */
            return(error);
        }
    } else {
        error = SMPcLUfac(ckt->CKTmatrix,ckt->CKTpivotAbsTol);
        if(error != 0) {
            if(error == E_SINGULAR) {
                /* the problem is that the matrix can't be solved with the
                 * current LU factorization.  Maybe if we reload and
                 * try to reorder again it will help...
                 */
                ckt->CKTniState |= NIACSHOULDREORDER;
                goto retry;
            }
            return(error); /* can't handle E_BADMATRIX, so let caller */
        }
    } 
    SMPcSolve(ckt->CKTmatrix,ckt->CKTrhs, 
            ckt->CKTirhs, ckt->CKTrhsSpare,
            ckt->CKTirhsSpare);

    *ckt->CKTrhs = 0;
    *ckt->CKTrhsSpare = 0;
    *ckt->CKTrhsOld = 0;
    *ckt->CKTirhs = 0;
    *ckt->CKTirhsSpare = 0;
    *ckt->CKTirhsOld = 0;

    temp = ckt->CKTirhsOld;
    ckt->CKTirhsOld = ckt->CKTirhs;
    ckt->CKTirhs = temp;

    temp = ckt->CKTrhsOld;
    ckt->CKTrhsOld = ckt->CKTrhs;
    ckt->CKTrhs = temp;
    return(OK);
}
