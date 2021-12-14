/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTsetBreak(ckt,time)
     *   add the given time to the breakpoint table for the given circuit
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "util.h"
#include "IFsim.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTsetBreak.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:21 $")

int
CKTsetBreak(ckt,time)
    register CKTcircuit *ckt;
    double time;

{
    double *tmp;
    register int i,j;

    if(ckt->CKTtime > time) {
        (*(SPfrontEnd->IFerror))(ERR_PANIC,"breakpoint in the past - HELP!",
                (IFuid *)NULL);
        return(E_INTERN);
    }
    for(i=0;i<ckt->CKTbreakSize;i++) {
        if(*(ckt->CKTbreaks+i)>time) { /* passed */
            if((*(ckt->CKTbreaks+i)-time) <= ckt->CKTminBreak) {
                /* very close together - take earlier point */
                *(ckt->CKTbreaks+i) = time;
                return(OK);
            }
            if(time-*(ckt->CKTbreaks+i-1) <= ckt->CKTminBreak) {
                /* very close together, but after, so skip */
                return(OK);
            }
            /* fits in middle - new array & insert */
            tmp = (double *)MALLOC((ckt->CKTbreakSize+1)*sizeof(double));
            if(tmp == (double *)NULL) return(E_NOMEM);
            for(j=0;j<i;j++) {
                *(tmp+j) = *(ckt->CKTbreaks+j);
            }
            *(tmp+i)=time;
            for(j=i;j<ckt->CKTbreakSize;j++) {
                *(tmp+j+1) = *(ckt->CKTbreaks+j);
            }
            FREE(ckt->CKTbreaks);
            ckt->CKTbreakSize++;
            ckt->CKTbreaks=tmp;
            return(OK);
        }
    }
    /* never found it - beyond end of time - extend out idea of time */
    if(time-ckt->CKTbreaks[ckt->CKTbreakSize-1]<=ckt->CKTminBreak) {
        /* very close tegether - keep earlier, throw out new point */
        return(OK);
    }
    /* fits at end - grow array & add on */
    ckt->CKTbreaks = (double *)REALLOC(ckt->CKTbreaks,
            (ckt->CKTbreakSize+1)*sizeof(double));
    ckt->CKTbreakSize++;
    ckt->CKTbreaks[ckt->CKTbreakSize-1]=time;
    return(OK);
}
