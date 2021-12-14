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

RCSID("BSIMtemp.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:13 $")

/* ARGSUSED */
int
BSIMtemp(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /* load the BSIM device structure with those pointers needed later 
         * for fast matrix loading 
         */

{
    register BSIMmodel *model = (BSIMmodel*) inModel;
    register BSIMinstance *here;
    double  EffChanLength;
    double EffChanWidth;
    double Cox;
    double CoxWoverL ;
    double Leff;    /* effective channel length im micron */
    double Weff;    /* effective channel width in micron */

    /*  loop through all the BSIM device models */
    for( ; model != NULL; model = model->BSIMnextModel ) {
    
/* Default value Processing for BSIM MOSFET Models */
        /* Some Limiting for Model Parameters */
        if( model->BSIMbulkJctPotential < 0.1)  {
            model->BSIMbulkJctPotential = 0.1;
        }
        if( model->BSIMsidewallJctPotential < 0.1)  {
            model->BSIMsidewallJctPotential = 0.1;
        }

        Cox = 3.453e-13/(model->BSIMoxideThickness * 1.0e-4);/*in F/cm**2 */
        model->BSIMCox = Cox;     /* unit:  F/cm**2  */

        /* loop through all the instances of the model */
        for (here = model->BSIMinstances; here != NULL ;
                here=here->BSIMnextInstance) {

            if( (EffChanLength = here->BSIMl - model->BSIMdeltaL *1e-6 )<=0) { 
                IFuid namarray[2];
                namarray[0] = model->BSIMmodName;
                namarray[1] = here->BSIMname;
                (*(SPfrontEnd->IFerror))(ERR_FATAL,
                    "BSIM: mosfet %s, model %s: Effective channel length <=0",
                    namarray);
                return(E_BADPARM);
            }
            if( (EffChanWidth = here->BSIMw - model->BSIMdeltaW *1e-6 ) <= 0 ) {
                IFuid namarray[2];
                namarray[0] = model->BSIMmodName;
                namarray[1] = here->BSIMname;
                (*(SPfrontEnd->IFerror))(ERR_FATAL,
                    "BSIM: mosfet %s, model %s: Effective channel width <=0",
                    namarray);
                return(E_BADPARM);
            }
            here->BSIMGDoverlapCap=EffChanWidth *model->BSIMgateDrainOverlapCap;
            here->BSIMGSoverlapCap=EffChanWidth*model->BSIMgateSourceOverlapCap;
            here->BSIMGBoverlapCap=here->BSIMl * model->BSIMgateBulkOverlapCap;

            /* process drain series resistance */
            if( (here->BSIMdrainConductance=model->BSIMsheetResistance *
                    here->BSIMdrainSquares) != 0.0 ) {   
                here->BSIMdrainConductance = 1. / here->BSIMdrainConductance ;
            } 
                   
            /* process source series resistance */
            if( (here->BSIMsourceConductance=model->BSIMsheetResistance *
                    here->BSIMsourceSquares) != 0.0 ) { 
                here->BSIMsourceConductance = 1. / here->BSIMsourceConductance ;
            }
                   
            Leff = EffChanLength * 1.e6; /* convert into micron */
            Weff = EffChanWidth * 1.e6; /* convert into micron */
            CoxWoverL = Cox * Weff / Leff ; /* F/cm**2 */

            here->BSIMvfb = model->BSIMvfb0 + 
                model->BSIMvfbL / Leff + model->BSIMvfbW / Weff;
            here->BSIMphi = model->BSIMphi0 +
                model->BSIMphiL / Leff + model->BSIMphiW / Weff;
            here->BSIMK1 = model->BSIMK10 +
                model->BSIMK1L / Leff + model->BSIMK1W / Weff;
            here->BSIMK2 = model->BSIMK20 +
                model->BSIMK2L / Leff + model->BSIMK2W / Weff;
            here->BSIMeta = model->BSIMeta0 +
                model->BSIMetaL / Leff + model->BSIMetaW / Weff;
            here->BSIMetaB = model->BSIMetaB0 +
                model->BSIMetaBl / Leff + model->BSIMetaBw / Weff;
            here->BSIMetaD = model->BSIMetaD0 +
                model->BSIMetaDl / Leff + model->BSIMetaDw / Weff;
            here->BSIMbetaZero = model->BSIMmobZero;
            here->BSIMbetaZeroB = model->BSIMmobZeroB0 + 
                model->BSIMmobZeroBl / Leff + model->BSIMmobZeroBw / Weff;
            here->BSIMugs = model->BSIMugs0 +
                model->BSIMugsL / Leff + model->BSIMugsW / Weff;
            here->BSIMugsB = model->BSIMugsB0 +
                model->BSIMugsBL / Leff + model->BSIMugsBW / Weff;
            here->BSIMuds = model->BSIMuds0 +
                model->BSIMudsL / Leff + model->BSIMudsW / Weff;
            here->BSIMudsB = model->BSIMudsB0 +
                model->BSIMudsBL / Leff + model->BSIMudsBW / Weff;
            here->BSIMudsD = model->BSIMudsD0 +
                model->BSIMudsDL / Leff + model->BSIMudsDW / Weff;
            here->BSIMbetaVdd = model->BSIMmobVdd0 +
                model->BSIMmobVddl / Leff + model->BSIMmobVddw / Weff;
            here->BSIMbetaVddB = model->BSIMmobVddB0 + 
                model->BSIMmobVddBl / Leff + model->BSIMmobVddBw / Weff;
            here->BSIMbetaVddD = model->BSIMmobVddD0 +
                model->BSIMmobVddDl / Leff + model->BSIMmobVddDw / Weff;
            here->BSIMsubthSlope = model->BSIMsubthSlope0 + 
                model->BSIMsubthSlopeL / Leff + model->BSIMsubthSlopeW / Weff;
            here->BSIMsubthSlopeB = model->BSIMsubthSlopeB0 +
                model->BSIMsubthSlopeBL / Leff + model->BSIMsubthSlopeBW / Weff;
            here->BSIMsubthSlopeD = model->BSIMsubthSlopeD0 + 
                model->BSIMsubthSlopeDL / Leff + model->BSIMsubthSlopeDW / Weff;

            if(here->BSIMphi < 0.1 ) here->BSIMphi = 0.1;
            if(here->BSIMK1 < 0.0) here->BSIMK1 = 0.0;
            if(here->BSIMK2 < 0.0) here->BSIMK2 = 0.0;

            here->BSIMvt0 = here->BSIMvfb + here->BSIMphi + here->BSIMK1 * 
                sqrt(here->BSIMphi) - here->BSIMK2 * here->BSIMphi;

            here->BSIMvon = here->BSIMvt0;  /* added for initialization*/

                /* process Beta Parameters (unit: A/V**2) */

            here->BSIMbetaZero = here->BSIMbetaZero * CoxWoverL;
            here->BSIMbetaZeroB = here->BSIMbetaZeroB * CoxWoverL;
            here->BSIMbetaVdd = here->BSIMbetaVdd * CoxWoverL;
            here->BSIMbetaVddB = here->BSIMbetaVddB * CoxWoverL;
            here->BSIMbetaVddD = MAX(here->BSIMbetaVddD * CoxWoverL,0.0);

        }
    }
    return(OK);
}  


