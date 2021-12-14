/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTclrBreak(ckt)
     *   delete the first time from the breakpoint table for the given circuit
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTclrBreak.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:15 $")

int
CKTclrBreak(ckt)
    register CKTcircuit *ckt;

{
    double *tmp;
    register int j;

    if(ckt->CKTbreakSize >2) {
        tmp = (double *)MALLOC((ckt->CKTbreakSize-1)*sizeof(double));
        if(tmp == (double *)NULL) return(E_NOMEM);
        for(j=1;j<ckt->CKTbreakSize;j++) {
            *(tmp+j-1) = *(ckt->CKTbreaks+j);
        }
        FREE(ckt->CKTbreaks);
        ckt->CKTbreakSize--;
        ckt->CKTbreaks=tmp;
    } else {
        *(ckt->CKTbreaks)= *(ckt->CKTbreaks+1);
        *(ckt->CKTbreaks+1) = ckt->CKTfinalTime;
    }
    return(OK);
}
