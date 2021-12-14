/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /*
     * NIconvTest(ckt)
     *  perform the convergence test - returns 1 if any of the 
     *  values in the old and new arrays have changed by more 
     *  than absTol + relTol*(max(old,new)), otherwise returns 0
     */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "SMPdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("NIconvTest.c $Revision: 1.3 $ on $Date: 91/08/05 11:56:37 $ ")

int
NIconvTest(ckt)
    register CKTcircuit *ckt;

{
    register int i; /* generic loop variable */
    register int size;  /* size of the matrix */
    register CKTnode *node; /* current matrix entry */
    double old;
    double new;
    double tol;

    node = ckt->CKTnodes;
    size = SMPmatSize(ckt->CKTmatrix);
    for (i=1;i<=size;i++) {
        node = node->next;
        new =  *((ckt->CKTrhs) + i ) ;
        old =  *((ckt->CKTrhsOld) + i ) ;
        if(node->type == 3) {
            tol =  ckt->CKTreltol * (MAX(FABS(old),FABS(new))) +
                    ckt->CKTvoltTol;
            if (FABS(new-old) >tol ) {
#ifdef STEPDEBUG
                printf(" non-convergence at node %s\n",CKTnodName(ckt,i));
#endif /* STEPDEBUG */
/* gtri - begin - wbk - report conv prob */
                if(ckt->enh->conv_debug.report_conv_probs)
                    ENHreport_conv_prob(ENH_ANALOG_NODE, CKTnodName(ckt,i), "");
/* gtri - end - wbk - report conv prob */
                return(1);
            }
        } else {
            tol =  ckt->CKTreltol * (MAX(FABS(old),FABS(new))) +
                    ckt->CKTabstol;
            if (FABS(new-old) >tol ) {
#ifdef STEPDEBUG
                printf(" non-convergence at node %s\n",CKTnodName(ckt,i));
#endif /* STEPDEBUG */
/* gtri - begin - wbk - report conv prob */
                if(ckt->enh->conv_debug.report_conv_probs)
                    ENHreport_conv_prob(ENH_ANALOG_BRANCH, CKTnodName(ckt,i), "");
/* gtri - end - wbk - report conv prob */
                return(1);
            }
        }
    }

#ifdef NEWCONV
    return(CKTconvTest(ckt));
#else /* NEWCONV */
    return(0);
#endif /* NEWCONV */
}
