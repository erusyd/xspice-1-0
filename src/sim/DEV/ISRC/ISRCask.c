/*
 * Copyright (c) 1988 Thomas L. Quarles
 */

/*
 * This routine gives access to the internal device parameters
 * of independent current SouRCe
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "ISRCdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("ISRCask.c $Revision: 1.2 $ on $Date: 91/11/08 16:22:07 $")

/* ARGSUSED */
int
ISRCask(ckt,inst,which,value,select)
    CKTcircuit *ckt;
    GENinstance *inst;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    ISRCinstance *here = (ISRCinstance*)inst;
    static char *msg = "Current and power not available in ac analysis";
    int temp;

    switch(which) {
        case ISRC_DC:
            value->rValue = here->ISRCdcValue;
            return (OK);
        case ISRC_AC_MAG:
            value->rValue = here->ISRCacMag;
            return (OK);
        case ISRC_AC_PHASE:
            value->rValue = here->ISRCacPhase;
            return (OK);
        case ISRC_PULSE:
        case ISRC_SINE:
        case ISRC_EXP:
        case ISRC_PWL:
        case ISRC_SFFM:
        case ISRC_FCN_COEFFS:
            temp = value->v.numValue = here->ISRCfunctionOrder;
            value->v.vec.rVec = (double *) 
                    MALLOC(here->ISRCfunctionOrder * sizeof(double));
            while (temp--) {
                *value->v.vec.rVec++ = *here->ISRCcoeffs++;
            }
            return (OK);
        case ISRC_NEG_NODE:
            value->iValue = here->ISRCnegNode;
            return (OK);
        case ISRC_POS_NODE:
            value->iValue = here->ISRCposNode;
            return (OK);
        case ISRC_FCN_TYPE:
            value->iValue = here->ISRCfunctionType;
        case ISRC_AC_REAL:
            value->rValue = here->ISRCacReal;
            return (OK);
        case ISRC_AC_IMAG:
            value->rValue = here->ISRCacImag;
            return (OK);
        case ISRC_FCN_ORDER:
            value->rValue = here->ISRCfunctionOrder;
            return (OK);
        case ISRC_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "ISRCask";
                strcpy(errMsg,msg);
                return(E_ASKPOWER);
            } else {
                value->rValue = -here->ISRCdcValue * 
                        (*(ckt->CKTrhsOld + here->ISRCposNode) -
                        *(ckt->CKTrhsOld + here->ISRCnegNode));
            }
            return(OK);
/* gtri - begin - add current value information */
        case ISRC_CURRENT:
            value->rValue = here->ISRCcurrent;
            return (OK);
/* gtri - end - add current value information */
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
