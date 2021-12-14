/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

/*
 * This routine gives access to the internal device parameters
 * of independent Voltage SouRCe
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "VSRCdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("VSRCask.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:33 $")

/* ARGSUSED */
int
VSRCask(ckt,inst,which,value,select)
    CKTcircuit *ckt;
    GENinstance *inst;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    VSRCinstance *here = (VSRCinstance*)inst;
    int temp;
    static char *msg =  "Current and power not available in ac analysis";

    switch(which) {
        case VSRC_DC:
            value->rValue = here->VSRCdcValue;
            return (OK);
        case VSRC_AC_MAG:
            value->rValue = here->VSRCacMag;
            return (OK);
        case VSRC_AC_PHASE:
            value->rValue = here->VSRCacPhase;
            return (OK);
        case VSRC_PULSE:
        case VSRC_SINE:
        case VSRC_EXP:
        case VSRC_PWL:
        case VSRC_SFFM:
        case VSRC_FCN_COEFFS:
            temp = value->v.numValue = here->VSRCfunctionOrder;
            value->v.vec.rVec = (double *) 
                    MALLOC (here->VSRCfunctionOrder * sizeof(double));
            while (temp--) {
                *value->v.vec.rVec++ = *here->VSRCcoeffs++;
            }
            return (OK);
        case VSRC_AC:
            value->v.numValue = 2;
            value->v.vec.rVec = (double *) 
                    MALLOC (value->v.numValue * sizeof (double));
            *value->v.vec.rVec++ = here->VSRCacMag;
            *value->v.vec.rVec = here->VSRCacPhase;
            return (OK);
        case VSRC_NEG_NODE:
            value->iValue = here->VSRCnegNode;
            return (OK);
        case VSRC_POS_NODE:
            value->iValue = here->VSRCposNode;
            return (OK);
        case VSRC_FCN_TYPE:
            value->iValue = here->VSRCfunctionType;
            return (OK);
        case VSRC_AC_REAL:
            value->rValue = here->VSRCacReal;
            return (OK);
        case VSRC_AC_IMAG:
            value->rValue = here->VSRCacImag;
            return (OK);
        case VSRC_FCN_ORDER:
            value->rValue = here->VSRCfunctionOrder;
            return (OK);
        case VSRC_CURRENT:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "VSRCask";
                strcpy(errMsg,msg);
                return(E_ASKCURRENT);
            } else {
                value->rValue = *(ckt->CKTrhsOld+here->VSRCbranch);
            }
            return(OK);
        case VSRC_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "VSRCask";
                strcpy(errMsg,msg);
                return(E_ASKPOWER);
            } else {
                value->rValue = (*(ckt->CKTrhsOld+here->VSRCposNode)
                        - *(ckt->CKTrhsOld + here->VSRCnegNode)) * 
                        -*(ckt->CKTrhsOld + here->VSRCbranch);
            }
            return(OK);
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
