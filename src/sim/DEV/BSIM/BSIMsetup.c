/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "BSIMdefs.h"
#include "util.h"
#include "CONST.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMsetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:12 $")

int
BSIMsetup(matrix,inModel,ckt,states)
    register SMPmatrix *matrix;
    register GENmodel *inModel;
    register CKTcircuit *ckt;
    int *states;
        /* load the BSIM device structure with those pointers needed later 
         * for fast matrix loading 
         */

{
    register BSIMmodel *model = (BSIMmodel*)inModel;
    register BSIMinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the BSIM device models */
    for( ; model != NULL; model = model->BSIMnextModel ) {
    
/* Default value Processing for BSIM MOSFET Models */
        if( ! model->BSIMtypeGiven) {
            model->BSIMtype = NMOS;  /* NMOS */
        }
        if( ! model->BSIMvfb0Given) {
            model->BSIMvfb0 = 0.0;
        }
        if( ! model->BSIMvfbLGiven) {
            model->BSIMvfbL = 0.0;
        }
        if( ! model->BSIMvfbWGiven) {
            model->BSIMvfbW = 0.0;
        }
        if( ! model->BSIMphi0Given) {
            model->BSIMphi0 = 0.0;
        }
        if( ! model->BSIMphiLGiven) {
            model->BSIMphiL = 0.0;
        }
        if( ! model->BSIMphiWGiven) {
            model->BSIMphiW = 0.0;
        }
        if( ! model->BSIMK10Given) {
            model->BSIMK10 = 0.0;
        }
        if( ! model->BSIMK1LGiven) {
            model->BSIMK1L = 0.0;
        }
        if( ! model->BSIMK1WGiven) {
            model->BSIMK1W = 0.0;
        }
        if( ! model->BSIMK20Given) {
            model->BSIMK20 = 0.0;
        }
        if( ! model->BSIMK2LGiven) {
            model->BSIMK2L = 0.0;
        }
        if( ! model->BSIMK2WGiven) {
            model->BSIMK2W = 0.0;
        }
        if( ! model->BSIMeta0Given) {
            model->BSIMeta0 = 0.0;
        }
        if( ! model->BSIMetaLGiven) {
            model->BSIMetaL = 0.0;
        }
        if( ! model->BSIMetaWGiven) {
            model->BSIMetaW = 0.0;
        }
        if( ! model->BSIMmobZeroGiven) {
            model->BSIMmobZero = 0.0;
        }
        if( ! model->BSIMdeltaLGiven) {
            model->BSIMdeltaL = 0.0;
        }
        if( ! model->BSIMdeltaWGiven) {
            model->BSIMdeltaW = 0.0;
        }
        if( ! model->BSIMugs0Given) {
            model->BSIMugs0 = 0.0;
        }
        if( ! model->BSIMugsLGiven) {
            model->BSIMugsL = 0.0;
        }
        if( ! model->BSIMugsWGiven) {
            model->BSIMugsW = 0.0;
        }
        if( ! model->BSIMuds0Given) {
            model->BSIMuds0 = 0.0;
        }
        if( ! model->BSIMudsLGiven) {
            model->BSIMudsL = 0.0;
        }
        if( ! model->BSIMudsWGiven) {
            model->BSIMudsW = 0.0;
        }
        if( ! model->BSIMmobZeroB0Given) {
            model->BSIMmobZeroB0 = 0.0;
        }
        if( ! model->BSIMmobZeroBlGiven) {
            model->BSIMmobZeroBl = 0.0;
        }
        if( ! model->BSIMmobZeroBwGiven) {
            model->BSIMmobZeroBw = 0.0;
        }
        if( ! model->BSIMetaB0Given) {
            model->BSIMetaB0 = 0.0;
        }
        if( ! model->BSIMetaBlGiven) {
            model->BSIMetaBl = 0.0;
        }
        if( ! model->BSIMetaBwGiven) {
            model->BSIMetaBw = 0.0;
        }
        if( ! model->BSIMetaD0Given) {
            model->BSIMetaD0 = 0.0;
        }
        if( ! model->BSIMetaDlGiven) {
            model->BSIMetaDl = 0.0;
        }
        if( ! model->BSIMetaDwGiven) {
            model->BSIMetaDw = 0.0;
        }
        if( ! model->BSIMugsB0Given) {
            model->BSIMugsB0 = 0.0;
        }
        if( ! model->BSIMugsBLGiven) {
            model->BSIMugsBL = 0.0;
        }
        if( ! model->BSIMugsBWGiven) {
            model->BSIMugsBW = 0.0;
        }
        if( ! model->BSIMudsB0Given) {
            model->BSIMudsB0 = 0.0;
        }
        if( ! model->BSIMudsBLGiven) {
            model->BSIMudsBL = 0.0;
        }
        if( ! model->BSIMudsBWGiven) {
            model->BSIMudsBW = 0.0;
        }
        if( ! model->BSIMmobVdd0Given) {
            model->BSIMmobVdd0 = 0.0;
        }
        if( ! model->BSIMmobVddlGiven) {
            model->BSIMmobVddl = 0.0;
        }
        if( ! model->BSIMmobVddwGiven) {
            model->BSIMmobVddw = 0.0;
        }
        if( ! model->BSIMmobVddB0Given) {
            model->BSIMmobVddB0 = 0.0;
        }
        if( ! model->BSIMmobVddBlGiven) {
            model->BSIMmobVddBl = 0.0;
        }
        if( ! model->BSIMmobVddBwGiven) {
            model->BSIMmobVddBw = 0.0;
        }
        if( ! model->BSIMmobVddD0Given) {
            model->BSIMmobVddD0 = 0.0;
        }
        if( ! model->BSIMmobVddDlGiven) {
            model->BSIMmobVddDl = 0.0;
        }
        if( ! model->BSIMmobVddDwGiven) {
            model->BSIMmobVddDw = 0.0;
        }
        if( ! model->BSIMudsD0Given) {
            model->BSIMudsD0 = 0.0;
        }
        if( ! model->BSIMudsDLGiven) {
            model->BSIMudsDL = 0.0;
        }
        if( ! model->BSIMudsDWGiven) {
            model->BSIMudsDW = 0.0;
        }
        if( ! model->BSIMoxideThicknessGiven) {
            model->BSIMoxideThickness = 0.0;  /* um */
        }
        if( ! model->BSIMtempGiven) {
            model->BSIMtemp = 0.0;
        }
        if( ! model->BSIMvddGiven) {
            model->BSIMvdd = 0.0;
        }
        if( ! model->BSIMgateDrainOverlapCapGiven) {
            model->BSIMgateDrainOverlapCap = 0.0;
        }
        if( ! model->BSIMgateSourceOverlapCapGiven) {
            model->BSIMgateSourceOverlapCap = 0.0;
        }
        if( ! model->BSIMgateBulkOverlapCapGiven) {
            model->BSIMgateBulkOverlapCap = 0.0;
        }
        if( ! model->BSIMchannelChargePartitionFlagGiven) {
            model->BSIMchannelChargePartitionFlag = 0.0;
        }
        if( ! model->BSIMsubthSlope0Given) {
            model->BSIMsubthSlope0 = 0.0;
        }
        if( ! model->BSIMsubthSlopeLGiven) {
            model->BSIMsubthSlopeL = 0.0;
        }
        if( ! model->BSIMsubthSlopeWGiven) {
            model->BSIMsubthSlopeW = 0.0;
        }
        if( ! model->BSIMsubthSlopeB0Given) {
            model->BSIMsubthSlopeB0 = 0.0;
        }
        if( ! model->BSIMsubthSlopeBLGiven) {
            model->BSIMsubthSlopeBL = 0.0;
        }
        if( ! model->BSIMsubthSlopeBWGiven) {
            model->BSIMsubthSlopeBW = 0.0;
        }
        if( ! model->BSIMsubthSlopeD0Given) {
            model->BSIMsubthSlopeD0 = 0.0;
        }
        if( ! model->BSIMsubthSlopeDLGiven) {
            model->BSIMsubthSlopeDL = 0.0;
        }
        if( ! model->BSIMsubthSlopeDWGiven) {
            model->BSIMsubthSlopeDW = 0.0;
        }
        if( ! model->BSIMsheetResistanceGiven) {
            model->BSIMsheetResistance = 0.0;
        }
        if( ! model->BSIMunitAreaJctCapGiven) {
            model->BSIMunitAreaJctCap = 0.0;
        }
        if( ! model->BSIMunitLengthSidewallJctCapGiven) {
            model->BSIMunitLengthSidewallJctCap = 0.0;
        }
        if( ! model->BSIMjctSatCurDensityGiven) {
            model->BSIMjctSatCurDensity = 0.0;
        }
        if( ! model->BSIMbulkJctPotentialGiven) {
            model->BSIMbulkJctPotential = 0.0;
        }
        if( ! model->BSIMsidewallJctPotentialGiven) {
            model->BSIMsidewallJctPotential = 0.0;
        }
        if( ! model->BSIMbulkJctBotGradingCoeffGiven) {
            model->BSIMbulkJctBotGradingCoeff = 0.0;
        }
        if( ! model->BSIMbulkJctSideGradingCoeffGiven) {
            model->BSIMbulkJctSideGradingCoeff = 0.0;
        }
        if( ! model->BSIMdefaultWidthGiven) {
            model->BSIMdefaultWidth = 0.0;
        }
        if( ! model->BSIMdeltaLengthGiven) {
            model->BSIMdeltaLength = 0.0;
        }

        /* loop through all the instances of the model */
        for (here = model->BSIMinstances; here != NULL ;
                here=here->BSIMnextInstance) {

            /* allocate a chunk of the state vector */
            here->BSIMstates = *states;
            *states += BSIMnumStates;

            /* perform the parameter defaulting */

            if(!here->BSIMdrainAreaGiven) {
                here->BSIMdrainArea = 0;
            }
            if(!here->BSIMdrainPerimeterGiven) {
                here->BSIMdrainPerimeter = 0;
            }
            if(!here->BSIMdrainSquaresGiven) {
                here->BSIMdrainSquares = 1;
            }
            if(!here->BSIMicVBSGiven) {
                here->BSIMicVBS = 0;
            }
            if(!here->BSIMicVDSGiven) {
                here->BSIMicVDS = 0;
            }
            if(!here->BSIMicVGSGiven) {
                here->BSIMicVGS = 0;
            }
            if(!here->BSIMlGiven) {
                here->BSIMl = 5e-6;
            }
            if(!here->BSIMsourceAreaGiven) {
                here->BSIMsourceArea = 0;
            }
            if(!here->BSIMsourcePerimeterGiven) {
                here->BSIMsourcePerimeter = 0;
            }
            if(!here->BSIMsourceSquaresGiven) {
                here->BSIMsourceSquares = 1;
            }
            if(!here->BSIMvdsatGiven) {
                here->BSIMvdsat = 0;
            }
            if(!here->BSIMvonGiven) {
                here->BSIMvon = 0;
            }
            if(!here->BSIMwGiven) {
                here->BSIMw = 5e-6;
            }

            /* process drain series resistance */
            if( (model->BSIMsheetResistance != 0) && 
                    (here->BSIMdrainSquares != 0.0 ) &&
                    (here->BSIMdNodePrime == 0) ) {
                error = CKTmkVolt(ckt,&tmp,here->BSIMname,"drain");
                if(error) return(error);
                here->BSIMdNodePrime = tmp->number;
            } else {
                    here->BSIMdNodePrime = here->BSIMdNode;
            }
                   
            /* process source series resistance */
            if( (model->BSIMsheetResistance != 0) && 
                    (here->BSIMsourceSquares != 0.0 ) &&
                    (here->BSIMsNodePrime == 0) ) {
                if(here->BSIMsNodePrime == 0) {
                    error = CKTmkVolt(ckt,&tmp,here->BSIMname,"source");
                    if(error) return(error);
                    here->BSIMsNodePrime = tmp->number;
                }
            } else  {
                here->BSIMsNodePrime = here->BSIMsNode;
            }
                   

        /* set Sparse Matrix Pointers */

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(BSIMDdPtr, BSIMdNode, BSIMdNode)
            TSTALLOC(BSIMGgPtr, BSIMgNode, BSIMgNode)
            TSTALLOC(BSIMSsPtr, BSIMsNode, BSIMsNode)
            TSTALLOC(BSIMBbPtr, BSIMbNode, BSIMbNode)
            TSTALLOC(BSIMDPdpPtr, BSIMdNodePrime, BSIMdNodePrime)
            TSTALLOC(BSIMSPspPtr, BSIMsNodePrime, BSIMsNodePrime)
            TSTALLOC(BSIMDdpPtr, BSIMdNode, BSIMdNodePrime)
            TSTALLOC(BSIMGbPtr, BSIMgNode, BSIMbNode)
            TSTALLOC(BSIMGdpPtr, BSIMgNode, BSIMdNodePrime)
            TSTALLOC(BSIMGspPtr, BSIMgNode, BSIMsNodePrime)
            TSTALLOC(BSIMSspPtr, BSIMsNode, BSIMsNodePrime)
            TSTALLOC(BSIMBdpPtr, BSIMbNode, BSIMdNodePrime)
            TSTALLOC(BSIMBspPtr, BSIMbNode, BSIMsNodePrime)
            TSTALLOC(BSIMDPspPtr, BSIMdNodePrime, BSIMsNodePrime)
            TSTALLOC(BSIMDPdPtr, BSIMdNodePrime, BSIMdNode)
            TSTALLOC(BSIMBgPtr, BSIMbNode, BSIMgNode)
            TSTALLOC(BSIMDPgPtr, BSIMdNodePrime, BSIMgNode)
            TSTALLOC(BSIMSPgPtr, BSIMsNodePrime, BSIMgNode)
            TSTALLOC(BSIMSPsPtr, BSIMsNodePrime, BSIMsNode)
            TSTALLOC(BSIMDPbPtr, BSIMdNodePrime, BSIMbNode)
            TSTALLOC(BSIMSPbPtr, BSIMsNodePrime, BSIMbNode)
            TSTALLOC(BSIMSPdpPtr, BSIMsNodePrime, BSIMdNodePrime)

        }
    }
    return(OK);
}  


