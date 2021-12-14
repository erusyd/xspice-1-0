/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "BSIMdefs.h"
#include "IFsim.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMmParam.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:09 $")

int
BSIMmParam(param,value,inMod)
    int param;
    IFvalue *value;
    GENmodel *inMod;
{
    BSIMmodel *mod = (BSIMmodel*)inMod;
    switch(param) {
        case  BSIM_MOD_VFB0 :
            mod->BSIMvfb0 = value->rValue;
            mod->BSIMvfb0Given = TRUE;
            break;
        case  BSIM_MOD_VFBL :
            mod->BSIMvfbL = value->rValue;
            mod->BSIMvfbLGiven = TRUE;
            break;
        case  BSIM_MOD_VFBW :
            mod->BSIMvfbW = value->rValue;
            mod->BSIMvfbWGiven = TRUE;
            break;
        case  BSIM_MOD_PHI0 :
            mod->BSIMphi0 = value->rValue;
            mod->BSIMphi0Given = TRUE;
            break;
        case  BSIM_MOD_PHIL :
            mod->BSIMphiL = value->rValue;
            mod->BSIMphiLGiven = TRUE;
            break;
        case  BSIM_MOD_PHIW :
            mod->BSIMphiW = value->rValue;
            mod->BSIMphiWGiven = TRUE;
            break;
        case  BSIM_MOD_K10 :
            mod->BSIMK10 = value->rValue;
            mod->BSIMK10Given = TRUE;
            break;
        case  BSIM_MOD_K1L :
            mod->BSIMK1L = value->rValue;
            mod->BSIMK1LGiven = TRUE;
            break;
        case  BSIM_MOD_K1W :
            mod->BSIMK1W = value->rValue;
            mod->BSIMK1WGiven = TRUE;
            break;
        case  BSIM_MOD_K20 :
            mod->BSIMK20 = value->rValue;
            mod->BSIMK20Given = TRUE;
            break;
        case  BSIM_MOD_K2L :
            mod->BSIMK2L = value->rValue;
            mod->BSIMK2LGiven = TRUE;
            break;
        case  BSIM_MOD_K2W :
            mod->BSIMK2W = value->rValue;
            mod->BSIMK2WGiven = TRUE;
            break;
        case  BSIM_MOD_ETA0 :
            mod->BSIMeta0 = value->rValue;
            mod->BSIMeta0Given = TRUE;
            break;
        case  BSIM_MOD_ETAL :
            mod->BSIMetaL = value->rValue;
            mod->BSIMetaLGiven = TRUE;
            break;
        case  BSIM_MOD_ETAW :
            mod->BSIMetaW = value->rValue;
            mod->BSIMetaWGiven = TRUE;
            break;
        case  BSIM_MOD_ETAB0 :
            mod->BSIMetaB0 = value->rValue;
            mod->BSIMetaB0Given = TRUE;
            break;
        case  BSIM_MOD_ETABL :
            mod->BSIMetaBl = value->rValue;
            mod->BSIMetaBlGiven = TRUE;
            break;
        case  BSIM_MOD_ETABW :
            mod->BSIMetaBw = value->rValue;
            mod->BSIMetaBwGiven = TRUE;
            break;
        case  BSIM_MOD_ETAD0 :
            mod->BSIMetaD0 = value->rValue;
            mod->BSIMetaD0Given = TRUE;
            break;
        case  BSIM_MOD_ETADL :
            mod->BSIMetaDl = value->rValue;
            mod->BSIMetaDlGiven = TRUE;
            break;
        case  BSIM_MOD_ETADW :
            mod->BSIMetaDw = value->rValue;
            mod->BSIMetaDwGiven = TRUE;
            break;
        case  BSIM_MOD_DELTAL :
            mod->BSIMdeltaL =  value->rValue;
            mod->BSIMdeltaLGiven = TRUE;
            break;
        case  BSIM_MOD_DELTAW :
            mod->BSIMdeltaW =  value->rValue;
            mod->BSIMdeltaWGiven = TRUE;
            break;
        case  BSIM_MOD_MOBZERO :
            mod->BSIMmobZero = value->rValue;
            mod->BSIMmobZeroGiven = TRUE;
            break;
        case  BSIM_MOD_MOBZEROB0 :
            mod->BSIMmobZeroB0 = value->rValue;
            mod->BSIMmobZeroB0Given = TRUE;
            break;
        case  BSIM_MOD_MOBZEROBL :
            mod->BSIMmobZeroBl = value->rValue;
            mod->BSIMmobZeroBlGiven = TRUE;
            break;
        case  BSIM_MOD_MOBZEROBW :
            mod->BSIMmobZeroBw = value->rValue;
            mod->BSIMmobZeroBwGiven = TRUE;
            break;
        case  BSIM_MOD_MOBVDD0 :
            mod->BSIMmobVdd0 = value->rValue;
            mod->BSIMmobVdd0Given = TRUE;
            break;
        case  BSIM_MOD_MOBVDDL :
            mod->BSIMmobVddl = value->rValue;
            mod->BSIMmobVddlGiven = TRUE;
            break;
        case  BSIM_MOD_MOBVDDW :
            mod->BSIMmobVddw = value->rValue;
            mod->BSIMmobVddwGiven = TRUE;
            break;
        case  BSIM_MOD_MOBVDDB0 :
            mod->BSIMmobVddB0 = value->rValue;
            mod->BSIMmobVddB0Given = TRUE;
            break;
        case  BSIM_MOD_MOBVDDBL :
            mod->BSIMmobVddBl = value->rValue;
            mod->BSIMmobVddBlGiven = TRUE;
            break;
        case  BSIM_MOD_MOBVDDBW :
            mod->BSIMmobVddBw = value->rValue;
            mod->BSIMmobVddBwGiven = TRUE;
            break;
        case  BSIM_MOD_MOBVDDD0 :
            mod->BSIMmobVddD0 = value->rValue;
            mod->BSIMmobVddD0Given = TRUE;
            break;
        case  BSIM_MOD_MOBVDDDL :
            mod->BSIMmobVddDl = value->rValue;
            mod->BSIMmobVddDlGiven = TRUE;
            break;
        case  BSIM_MOD_MOBVDDDW :
            mod->BSIMmobVddDw = value->rValue;
            mod->BSIMmobVddDwGiven = TRUE;
            break;
        case  BSIM_MOD_UGS0 :
            mod->BSIMugs0 = value->rValue;
            mod->BSIMugs0Given = TRUE;
            break;
        case  BSIM_MOD_UGSL :
            mod->BSIMugsL = value->rValue;
            mod->BSIMugsLGiven = TRUE;
            break;
        case  BSIM_MOD_UGSW :
            mod->BSIMugsW = value->rValue;
            mod->BSIMugsWGiven = TRUE;
            break;
        case  BSIM_MOD_UGSB0 :
            mod->BSIMugsB0 = value->rValue;
            mod->BSIMugsB0Given = TRUE;
            break;
        case  BSIM_MOD_UGSBL :
            mod->BSIMugsBL = value->rValue;
            mod->BSIMugsBLGiven = TRUE;
            break;
        case  BSIM_MOD_UGSBW :
            mod->BSIMugsBW = value->rValue;
            mod->BSIMugsBWGiven = TRUE;
            break;
        case  BSIM_MOD_UDS0 :
            mod->BSIMuds0 = value->rValue;
            mod->BSIMuds0Given = TRUE;
            break;
        case  BSIM_MOD_UDSL :
            mod->BSIMudsL = value->rValue;
            mod->BSIMudsLGiven = TRUE;
            break;
        case  BSIM_MOD_UDSW :
            mod->BSIMudsW = value->rValue;
            mod->BSIMudsWGiven = TRUE;
            break;
        case  BSIM_MOD_UDSB0 :
            mod->BSIMudsB0 = value->rValue;
            mod->BSIMudsB0Given = TRUE;
            break;
        case  BSIM_MOD_UDSBL :
            mod->BSIMudsBL = value->rValue;
            mod->BSIMudsBLGiven = TRUE;
            break;
        case  BSIM_MOD_UDSBW :
            mod->BSIMudsBW = value->rValue;
            mod->BSIMudsBWGiven = TRUE;
            break;
        case  BSIM_MOD_UDSD0 :
            mod->BSIMudsD0 = value->rValue;
            mod->BSIMudsD0Given = TRUE;
            break;
        case  BSIM_MOD_UDSDL :
            mod->BSIMudsDL = value->rValue;
            mod->BSIMudsDLGiven = TRUE;
            break;
        case  BSIM_MOD_UDSDW :
            mod->BSIMudsDW = value->rValue;
            mod->BSIMudsDWGiven = TRUE;
            break;
        case  BSIM_MOD_N00 :
            mod->BSIMsubthSlope0 = value->rValue;
            mod->BSIMsubthSlope0Given = TRUE;
            break;
        case  BSIM_MOD_N0L :
            mod->BSIMsubthSlopeL = value->rValue;
            mod->BSIMsubthSlopeLGiven = TRUE;
            break;
        case  BSIM_MOD_N0W :
            mod->BSIMsubthSlopeW = value->rValue;
            mod->BSIMsubthSlopeWGiven = TRUE;
            break;
        case  BSIM_MOD_NB0 :
            mod->BSIMsubthSlopeB0 = value->rValue;
            mod->BSIMsubthSlopeB0Given = TRUE;
            break;
        case  BSIM_MOD_NBL :
            mod->BSIMsubthSlopeBL = value->rValue;
            mod->BSIMsubthSlopeBLGiven = TRUE;
            break;
        case  BSIM_MOD_NBW :
            mod->BSIMsubthSlopeBW = value->rValue;
            mod->BSIMsubthSlopeBWGiven = TRUE;
            break;
        case  BSIM_MOD_ND0 :
            mod->BSIMsubthSlopeD0 = value->rValue;
            mod->BSIMsubthSlopeD0Given = TRUE;
            break;
        case  BSIM_MOD_NDL :
            mod->BSIMsubthSlopeDL = value->rValue;
            mod->BSIMsubthSlopeDLGiven = TRUE;
            break;
        case  BSIM_MOD_NDW :
            mod->BSIMsubthSlopeDW = value->rValue;
            mod->BSIMsubthSlopeDWGiven = TRUE;
            break;
        case  BSIM_MOD_TOX :
            mod->BSIMoxideThickness = value->rValue;
            mod->BSIMoxideThicknessGiven = TRUE;
            break;
        case  BSIM_MOD_TEMP :
            mod->BSIMtemp = value->rValue;
            mod->BSIMtempGiven = TRUE;
            break;
        case  BSIM_MOD_VDD :
            mod->BSIMvdd = value->rValue;
            mod->BSIMvddGiven = TRUE;
            break;
        case  BSIM_MOD_CGSO :
            mod->BSIMgateSourceOverlapCap = value->rValue;
            mod->BSIMgateSourceOverlapCapGiven = TRUE;
            break;
        case  BSIM_MOD_CGDO :
            mod->BSIMgateDrainOverlapCap = value->rValue;
            mod->BSIMgateDrainOverlapCapGiven = TRUE;
            break;
        case  BSIM_MOD_CGBO :
            mod->BSIMgateBulkOverlapCap = value->rValue;
            mod->BSIMgateBulkOverlapCapGiven = TRUE;
            break;
        case  BSIM_MOD_XPART :
            mod->BSIMchannelChargePartitionFlag = value->rValue;
            mod->BSIMchannelChargePartitionFlagGiven = TRUE;
            break;
        case  BSIM_MOD_RSH :
            mod->BSIMsheetResistance = value->rValue;
            mod->BSIMsheetResistanceGiven = TRUE;
            break;
        case  BSIM_MOD_JS :
            mod->BSIMjctSatCurDensity = value->rValue;
            mod->BSIMjctSatCurDensityGiven = TRUE;
            break;
        case  BSIM_MOD_PB :
            mod->BSIMbulkJctPotential = value->rValue;
            mod->BSIMbulkJctPotentialGiven = TRUE;
            break;
        case  BSIM_MOD_MJ :
            mod->BSIMbulkJctBotGradingCoeff = value->rValue;
            mod->BSIMbulkJctBotGradingCoeffGiven = TRUE;
            break;
        case  BSIM_MOD_PBSW :
            mod->BSIMsidewallJctPotential = value->rValue;
            mod->BSIMsidewallJctPotentialGiven = TRUE;
            break;
        case  BSIM_MOD_MJSW :
            mod->BSIMbulkJctSideGradingCoeff = value->rValue;
            mod->BSIMbulkJctSideGradingCoeffGiven = TRUE;
            break;
        case  BSIM_MOD_CJ :
            mod->BSIMunitAreaJctCap = value->rValue;
            mod->BSIMunitAreaJctCapGiven = TRUE;
            break;
        case  BSIM_MOD_CJSW :
            mod->BSIMunitLengthSidewallJctCap = value->rValue;
            mod->BSIMunitLengthSidewallJctCapGiven = TRUE;
            break;
        case  BSIM_MOD_DEFWIDTH :
            mod->BSIMdefaultWidth = value->rValue;
            mod->BSIMdefaultWidthGiven = TRUE;
            break;
        case  BSIM_MOD_DELLENGTH :
            mod->BSIMdeltaLength = value->rValue;
            mod->BSIMdeltaLengthGiven = TRUE;
            break;
        case  BSIM_MOD_NMOS  :
            if(value->iValue) {
                mod->BSIMtype = 1;
                mod->BSIMtypeGiven = TRUE;
            }
            break;
        case  BSIM_MOD_PMOS  :
            if(value->iValue) {
                mod->BSIMtype = - 1;
                mod->BSIMtypeGiven = TRUE;
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}


