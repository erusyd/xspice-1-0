/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTop.c $Revision: 1.3 $ on $Date: 91/08/01 17:18:38 $")

int
CKTop(ckt,firstmode, continuemode, iterlim) 
    CKTcircuit *ckt;
    long firstmode;    
    long continuemode;    
    int iterlim;
{
    int converged;
    int i;

    ckt->CKTmode = firstmode;
    if(!ckt->CKTnoOpIter) {
/* gtri - begin - wbk - add convergence problem reporting flags */
        if((ckt->CKTnumGminSteps <= 0) && (ckt->CKTnumSrcSteps <= 0))
            ckt->enh->conv_debug.last_NIiter_call = MIF_TRUE;
        else
            ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - end - wbk - add convergence problem reporting flags */
        converged = NIiter(ckt,iterlim);
    } else {
        converged = 1;  /* the 'go directly to gmin stepping' option */
    }
    if(converged != 0) {
        /* no convergence on the first try, so we do something else */
        /* first, check if we should try gmin stepping */
        /* note that no path out of this code allows ckt->CKTdiagGmin to be 
         * anything but 0.000000000
         */
        if(ckt->CKTnumGminSteps >1) {
            ckt->CKTmode = firstmode;
            (*(SPfrontEnd->IFerror))(ERR_INFO,
                    "starting Gmin stepping",(IFuid *)NULL);
            ckt->CKTdiagGmin = ckt->CKTgmin;
            for(i=0;i<ckt->CKTnumGminSteps;i++) {
                ckt->CKTdiagGmin *= 10;
            }
            for(i=0;i<=ckt->CKTnumGminSteps;i++) {
                ckt->CKTnoncon =1;
                converged = NIiter(ckt,iterlim);
                if(converged != 0) {
                    ckt->CKTdiagGmin = 0;
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "Gmin step failed",(IFuid *)NULL);
                    break;
                }
                ckt->CKTdiagGmin /= 10;
                ckt->CKTmode=continuemode;
                (*(SPfrontEnd->IFerror))(ERR_INFO,
                        "One sucessful Gmin step",(IFuid *)NULL);
            }
            ckt->CKTdiagGmin = 0;
/* gtri - begin - wbk - add convergence problem reporting flags */
            if(ckt->CKTnumSrcSteps <= 0)
                ckt->enh->conv_debug.last_NIiter_call = MIF_TRUE;
            else
                ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - end - wbk - add convergence problem reporting flags */
            converged = NIiter(ckt,iterlim);
            if(converged == 0) {
                (*(SPfrontEnd->IFerror))(ERR_INFO,
                        "Gmin stepping completed",(IFuid *)NULL);
/* gtri - begin - wbk - add convergence problem reporting flags */
                ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - end - wbk - add convergence problem reporting flags */
                return(0);
            }
            (*(SPfrontEnd->IFerror))(ERR_WARNING,
                    "Gmin stepping failed",(IFuid *)NULL);

        /* now, we'll try source stepping - we scale the sources
         * to 0, converge, then start stepping them up until they
         * are at their normal values
         */
        /* note that no path out of this code allows ckt->CKTsrcFact to be 
         * anything but 1.000000000
         */
        }
        if(ckt->CKTnumSrcSteps >1) {
            ckt->CKTmode = firstmode;
            (*(SPfrontEnd->IFerror))(ERR_INFO,
                    "starting source stepping",(IFuid *)NULL);
            for(i=0;i<=ckt->CKTnumSrcSteps;i++) {
                ckt->CKTsrcFact = ((double)i)/((double)ckt->CKTnumSrcSteps);
/* gtri - begin - wbk - add convergence problem reporting flags */
                ckt->enh->conv_debug.last_NIiter_call = MIF_TRUE;
/* gtri - end - wbk - add convergence problem reporting flags */
                converged = NIiter(ckt,iterlim);
                ckt->CKTmode = continuemode;
                if(converged != 0) {
                    ckt->CKTsrcFact = 1;
                    ckt->CKTcurrentAnalysis = DOING_TRAN;
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "source stepping failed",(IFuid *)NULL);
/* gtri - begin - wbk - add convergence problem reporting flags */
                    ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - end - wbk - add convergence problem reporting flags */
                    return(converged);
                }
                (*(SPfrontEnd->IFerror))(ERR_INFO,
                        "One successful source step",(IFuid *)NULL);
            }
            (*(SPfrontEnd->IFerror))(ERR_INFO,
                    "Source stepping completed",(IFuid *)NULL);
            ckt->CKTsrcFact = 1;
/* gtri - begin - wbk - add convergence problem reporting flags */
            ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - end - wbk - add convergence problem reporting flags */
            return(0);
        } else {
/* gtri - begin - wbk - add convergence problem reporting flags */
            ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - end - wbk - add convergence problem reporting flags */
            return(converged);
        }
    } 
/* gtri - begin - wbk - add convergence problem reporting flags */
    ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - end - wbk - add convergence problem reporting flags */
    return(0);
}
