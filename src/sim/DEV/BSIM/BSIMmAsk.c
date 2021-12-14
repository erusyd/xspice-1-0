/*
 * Copyright (c) 1988 Hong J. Park
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "BSIMdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMmAsk.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:07 $")

/*ARGSUSED*/
int
BSIMmAsk(ckt,inst,which,value)
    CKTcircuit *ckt;
    GENmodel *inst;
    int which;
    IFvalue *value;
{
    BSIMmodel *model = (BSIMmodel *)inst;
        switch(which) {
        case BSIM_MOD_VFB0: 
            value->rValue = model->BSIMvfb0; 
            return(OK);
        case BSIM_MOD_VFBL:
            value->rValue = model->BSIMvfbL; 
            return(OK);
        case BSIM_MOD_VFBW:
            value->rValue = model->BSIMvfbW; 
            return(OK);
        case BSIM_MOD_PHI0:
            value->rValue = model->BSIMphi0; 
            return(OK);
        case BSIM_MOD_PHIL:
            value->rValue = model->BSIMphiL; 
            return(OK);
        case BSIM_MOD_PHIW:
            value->rValue = model->BSIMphiW; 
            return(OK);
        case BSIM_MOD_K10:
            value->rValue = model->BSIMK10; 
            return(OK);
        case BSIM_MOD_K1L:
            value->rValue = model->BSIMK1L; 
            return(OK);
        case BSIM_MOD_K1W:
            value->rValue = model->BSIMK1W; 
            return(OK);
        case BSIM_MOD_K20:
            value->rValue = model->BSIMK20; 
            return(OK);
        case BSIM_MOD_K2L:
            value->rValue = model->BSIMK2L; 
            return(OK);
        case BSIM_MOD_K2W:
            value->rValue = model->BSIMK2W; 
            return(OK);
        case BSIM_MOD_ETA0:
            value->rValue = model->BSIMeta0; 
            return(OK);
        case BSIM_MOD_ETAL:
            value->rValue = model->BSIMetaL; 
            return(OK);
        case BSIM_MOD_ETAW:
            value->rValue = model->BSIMetaW; 
            return(OK);
        case BSIM_MOD_ETAB0:
            value->rValue = model->BSIMetaB0; 
            return(OK);
        case BSIM_MOD_ETABL:
            value->rValue = model->BSIMetaBl; 
            return(OK);
        case BSIM_MOD_ETABW:
            value->rValue = model->BSIMetaBw; 
            return(OK);
        case BSIM_MOD_ETAD0:
            value->rValue = model->BSIMetaD0; 
            return(OK);
        case BSIM_MOD_ETADL:
            value->rValue = model->BSIMetaDl; 
            return(OK);
        case BSIM_MOD_ETADW:
            value->rValue = model->BSIMetaDw; 
            return(OK);
        case BSIM_MOD_DELTAL:
            value->rValue = model->BSIMdeltaL; 
            return(OK);
        case BSIM_MOD_DELTAW:
            value->rValue = model->BSIMdeltaW; 
            return(OK);
        case BSIM_MOD_MOBZERO:
            value->rValue = model->BSIMmobZero; 
            return(OK);
        case BSIM_MOD_MOBZEROB0:
            value->rValue = model->BSIMmobZeroB0; 
            return(OK);
        case BSIM_MOD_MOBZEROBL:
            value->rValue = model->BSIMmobZeroBl; 
            return(OK);
        case BSIM_MOD_MOBZEROBW:
            value->rValue = model->BSIMmobZeroBw; 
            return(OK);
        case BSIM_MOD_MOBVDD0:
            value->rValue = model->BSIMmobVdd0; 
            return(OK);
        case BSIM_MOD_MOBVDDL:
            value->rValue = model->BSIMmobVddl; 
            return(OK);
        case BSIM_MOD_MOBVDDW:
            value->rValue = model->BSIMmobVddw; 
            return(OK);
        case BSIM_MOD_MOBVDDB0:
            value->rValue = model->BSIMmobVddB0; 
            return(OK);
        case BSIM_MOD_MOBVDDBL:
            value->rValue = model->BSIMmobVddBl; 
            return(OK);
        case BSIM_MOD_MOBVDDBW:
            value->rValue = model->BSIMmobVddBw; 
            return(OK);
        case BSIM_MOD_MOBVDDD0:
            value->rValue = model->BSIMmobVddD0; 
            return(OK);
        case BSIM_MOD_MOBVDDDL:
            value->rValue = model->BSIMmobVddDl; 
            return(OK);
        case BSIM_MOD_MOBVDDDW:
            value->rValue = model->BSIMmobVddDw; 
            return(OK);
        case BSIM_MOD_UGS0:
            value->rValue = model->BSIMugs0; 
            return(OK);
        case BSIM_MOD_UGSL:
            value->rValue = model->BSIMugsL; 
            return(OK);
        case BSIM_MOD_UGSW:
            value->rValue = model->BSIMugsW; 
            return(OK);
        case BSIM_MOD_UGSB0:
            value->rValue = model->BSIMugsB0; 
            return(OK);
        case BSIM_MOD_UGSBL:
            value->rValue = model->BSIMugsBL; 
            return(OK);
        case BSIM_MOD_UGSBW:
            value->rValue = model->BSIMugsBW; 
            return(OK);
        case BSIM_MOD_UDS0:
            value->rValue = model->BSIMuds0; 
            return(OK);
        case BSIM_MOD_UDSL:
            value->rValue = model->BSIMudsL; 
            return(OK);
        case BSIM_MOD_UDSW:
            value->rValue = model->BSIMudsW; 
            return(OK);
        case BSIM_MOD_UDSB0:
            value->rValue = model->BSIMudsB0; 
            return(OK);
        case BSIM_MOD_UDSBL:
            value->rValue = model->BSIMudsBL; 
            return(OK);
        case BSIM_MOD_UDSBW:
            value->rValue = model->BSIMudsBW; 
            return(OK);
        case BSIM_MOD_UDSD0:
            value->rValue = model->BSIMudsD0; 
            return(OK);
        case BSIM_MOD_UDSDL:
            value->rValue = model->BSIMudsDL; 
            return(OK);
        case BSIM_MOD_UDSDW:
            value->rValue = model->BSIMudsDW; 
            return(OK);
        case BSIM_MOD_N00:
            value->rValue = model->BSIMsubthSlope0; 
            return(OK);
        case BSIM_MOD_N0L:
            value->rValue = model->BSIMsubthSlopeL; 
            return(OK);
        case BSIM_MOD_N0W:
            value->rValue = model->BSIMsubthSlopeW; 
            return(OK);
        case BSIM_MOD_NB0:
            value->rValue = model->BSIMsubthSlopeB0; 
            return(OK);
        case BSIM_MOD_NBL:
            value->rValue = model->BSIMsubthSlopeBL; 
            return(OK);
        case BSIM_MOD_NBW:
            value->rValue = model->BSIMsubthSlopeBW; 
            return(OK);
        case BSIM_MOD_ND0:
            value->rValue = model->BSIMsubthSlopeD0; 
            return(OK);
        case BSIM_MOD_NDL:
            value->rValue = model->BSIMsubthSlopeDL; 
            return(OK);
        case BSIM_MOD_NDW:
            value->rValue = model->BSIMsubthSlopeDW; 
            return(OK);
        case BSIM_MOD_TOX:
            value->rValue = model->BSIMoxideThickness; 
            return(OK);
        case BSIM_MOD_TEMP:
            value->rValue = model->BSIMtemp; 
            return(OK);
        case BSIM_MOD_VDD:
            value->rValue = model->BSIMvdd; 
            return(OK);
        case BSIM_MOD_CGSO:
            value->rValue = model->BSIMgateSourceOverlapCap; 
            return(OK);
        case BSIM_MOD_CGDO:
            value->rValue = model->BSIMgateDrainOverlapCap; 
            return(OK);
        case BSIM_MOD_CGBO:
            value->rValue = model->BSIMgateBulkOverlapCap; 
            return(OK);
        case BSIM_MOD_XPART:
            value->rValue = model->BSIMchannelChargePartitionFlag; 
            return(OK);
        case BSIM_MOD_RSH:
            value->rValue = model->BSIMsheetResistance; 
            return(OK);
        case BSIM_MOD_JS:
            value->rValue = model->BSIMjctSatCurDensity; 
            return(OK);
        case BSIM_MOD_PB:
            value->rValue = model->BSIMbulkJctPotential; 
            return(OK);
        case BSIM_MOD_MJ:
            value->rValue = model->BSIMbulkJctBotGradingCoeff; 
            return(OK);
        case BSIM_MOD_PBSW:
            value->rValue = model->BSIMsidewallJctPotential; 
            return(OK);
        case BSIM_MOD_MJSW:
            value->rValue = model->BSIMbulkJctSideGradingCoeff; 
            return(OK);
        case BSIM_MOD_CJ:
            value->rValue = model->BSIMunitAreaJctCap; 
            return(OK);
        case BSIM_MOD_CJSW:
            value->rValue = model->BSIMunitLengthSidewallJctCap; 
            return(OK);
        case BSIM_MOD_DEFWIDTH:
            value->rValue = model->BSIMdefaultWidth; 
            return(OK);
        case BSIM_MOD_DELLENGTH:
            value->rValue = model->BSIMdeltaLength; 
            return(OK);
        default:
            return(E_BADPARM);
    }
    /* NOTREACHED */
}

