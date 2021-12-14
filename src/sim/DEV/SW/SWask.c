/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

/*
 * This routine gives access to the internal device parameters
 * of voltage controlled SWitch
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "SWdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("SWask.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:19 $")

/* ARGSUSED */
int
SWask(ckt,inst,which,value,select)
    CKTcircuit *ckt;
    GENinstance *inst;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    SWinstance *here = (SWinstance *)inst;
    static char *msg = "Current and power not available in ac analysis";

    switch(which) {
        case SW_POS_NODE:
            value->iValue = here->SWposNode;
            return (OK);
        case SW_NEG_NODE:
            value->iValue = here->SWnegNode;
            return (OK);
        case SW_POS_CONT_NODE:
            value->iValue = here->SWposCntrlNode;
            return (OK);
        case SW_NEG_CONT_NODE:
            value->iValue = here->SWnegCntrlNode;
            return (OK);
        case SW_CURRENT:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "SWask";
                strcpy(errMsg,msg);
                return(E_ASKCURRENT);
            } else {
                value->rValue = (*(ckt->CKTrhsOld + here->SWposNode)
                        - *(ckt->CKTrhsOld + here->SWnegNode)) *
                        here->SWcond;
            }
            return(OK);
        case SW_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "SWask";
                strcpy(errMsg,msg);
                return(E_ASKPOWER);
            } else {
                value->rValue = (*(ckt->CKTrhsOld + here->SWposNode)
                        - *(ckt->CKTrhsOld + here->SWnegNode)) *
                        (*(ckt->CKTrhsOld + here->SWposNode)
                        - *(ckt->CKTrhsOld + here->SWnegNode)) *
                        here->SWcond;
            }
            return(OK);
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
