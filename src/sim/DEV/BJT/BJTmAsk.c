/*
 * Copyright (c) 1987 Mathew Lew and Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CONST.h"
#include "IFsim.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "BJTdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BJTmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:42 $")

/*ARGSUSED*/
int
BJTmAsk(ckt,instPtr,which,value)
    CKTcircuit *ckt;
    GENmodel *instPtr;
    int which;
    IFvalue *value;
{
    BJTmodel *here = (BJTmodel*)instPtr;

    switch(which) {
        case BJT_MOD_TNOM:
            value->rValue = here->BJTtnom-CONSTCtoK;
            return(OK);
        case BJT_MOD_IS:
            value->rValue = here->BJTsatCur;
            return(OK);
        case BJT_MOD_BF:
            value->rValue = here->BJTbetaF;
            return(OK);
        case BJT_MOD_NF:
            value->rValue = here->BJTemissionCoeffF;
            return(OK);
        case BJT_MOD_VAF:
            value->rValue = here->BJTearlyVoltF;
            return(OK);
        case BJT_MOD_IKF:
            value->rValue = here->BJTrollOffF;
            return(OK);
        case BJT_MOD_ISE:
            value->rValue = here->BJTleakBEcurrent;
            return(OK);
        case BJT_MOD_C2:
            value->rValue = here->BJTc2;
            return(OK);
        case BJT_MOD_NE:
            value->rValue = here->BJTleakBEemissionCoeff;
            return(OK);
        case BJT_MOD_BR:
            value->rValue = here->BJTbetaR;
            return(OK);
        case BJT_MOD_NR:
            value->rValue = here->BJTemissionCoeffR;
            return(OK);
        case BJT_MOD_VAR:
            value->rValue = here->BJTearlyVoltR;
            return(OK);
        case BJT_MOD_IKR:
            value->rValue = here->BJTrollOffR;
            return(OK);
        case BJT_MOD_ISC:
            value->rValue = here->BJTleakBCcurrent;
            return(OK);
        case BJT_MOD_C4:
            value->rValue = here->BJTc4;
            return(OK);
        case BJT_MOD_NC:
            value->rValue = here->BJTleakBCemissionCoeff;
            return(OK);
        case BJT_MOD_RB:
            value->rValue = here->BJTbaseResist;
            return(OK);
        case BJT_MOD_IRB:
            value->rValue = here->BJTbaseCurrentHalfResist;
            return(OK);
        case BJT_MOD_RBM:
            value->rValue = here->BJTminBaseResist;
            return(OK);
        case BJT_MOD_RE:
            value->rValue = here->BJTemitterResist;
            return(OK);
        case BJT_MOD_RC:
            value->rValue = here->BJTcollectorResist;
            return(OK);
        case BJT_MOD_CJE:
            value->rValue = here->BJTdepletionCapBE;
            return(OK);
        case BJT_MOD_VJE:
            value->rValue = here->BJTpotentialBE;
            return(OK);
        case BJT_MOD_MJE:
            value->rValue = here->BJTjunctionExpBE;
            return(OK);
        case BJT_MOD_TF:
            value->rValue = here->BJTtransitTimeF;
            return(OK);
        case BJT_MOD_XTF:
            value->rValue = here->BJTtransitTimeBiasCoeffF;
            return(OK);
        case BJT_MOD_VTF:
            value->rValue = here->BJTtransitTimeFVBC;
            return(OK);
        case BJT_MOD_ITF:
            value->rValue = here->BJTtransitTimeHighCurrentF;
            return(OK);
        case BJT_MOD_PTF:
            value->rValue = here->BJTexcessPhase;
            return(OK);
        case BJT_MOD_CJC:
            value->rValue = here->BJTdepletionCapBC;
            return(OK);
        case BJT_MOD_VJC:
            value->rValue = here->BJTpotentialBC;
            return(OK);
        case BJT_MOD_MJC:
            value->rValue = here->BJTjunctionExpBC;
            return(OK);
        case BJT_MOD_XCJC:
            value->rValue = here->BJTbaseFractionBCcap;
            return(OK);
        case BJT_MOD_TR:
            value->rValue = here->BJTtransitTimeR;
            return(OK);
        case BJT_MOD_CJS:
            value->rValue = here->BJTcapCS;
            return(OK);
        case BJT_MOD_VJS:
            value->rValue = here->BJTpotentialSubstrate;
            return(OK);
        case BJT_MOD_MJS:
            value->rValue = here->BJTexponentialSubstrate;
            return(OK);
        case BJT_MOD_XTB:
            value->rValue = here->BJTbetaExp;
            return(OK);
        case BJT_MOD_EG:
            value->rValue = here->BJTenergyGap;
            return(OK);
        case BJT_MOD_XTI:
            value->rValue = here->BJTtempExpIS;
            return(OK);
        case BJT_MOD_FC:
            value->rValue = here->BJTdepletionCapCoeff;
            return(OK);
        case BJT_MOD_INVEARLYF:
            value->rValue = here->BJTinvEarlyVoltF;
            return(OK);
        case BJT_MOD_INVEARLYR:
            value->rValue = here->BJTinvEarlyVoltR;
            return(OK);
        case BJT_MOD_INVROLLOFFF:
            value->rValue = here->BJTinvRollOffF;
            return(OK);
        case BJT_MOD_INVROLLOFFR:
            value->rValue = here->BJTinvRollOffR;
            return(OK);
        case BJT_MOD_COLCONDUCT:
            value->rValue = here->BJTcollectorConduct;
            return(OK);
        case BJT_MOD_EMITTERCONDUCT:
            value->rValue = here->BJTemitterConduct;
            return(OK);
        case BJT_MOD_TRANSVBCFACT:
            value->rValue = here->BJTtransitTimeVBCFactor;
            return(OK);
        case BJT_MOD_EXCESSPHASEFACTOR:
            value->rValue = here->BJTexcessPhaseFactor;
            return(OK);
        default:
            return(E_BADPARM);
    }
    /* NOTREACHED */
}

