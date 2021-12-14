/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "CKTdefs.h"
#include "BSIMdefs.h"
#include "TRANdefs.h"
#include "CONST.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMconvTest.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:02 $")

int
BSIMconvTest(inModel,ckt)

    GENmodel *inModel;
    register CKTcircuit *ckt;

        /* actually load the current value into the 
         * sparse matrix previously provided 
         */
{
    register BSIMmodel *model = (BSIMmodel*)inModel;
    register BSIMinstance *here;
    double cbd;
    double cbhat;
    double cbs;
    double cd;
    double cdhat;
    double delvbd;
    double delvbs;
    double delvds;
    double delvgd;
    double delvgs;
    double tol;
    double vbd;
    double vbs;
    double vds;
    double vgd;
    double vgdo;
    double vgs;


    /*  loop through all the BSIM device models */
    for( ; model != NULL; model = model->BSIMnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->BSIMinstances; here != NULL ;
                here=here->BSIMnextInstance) {

            vbs = model->BSIMtype * ( 
                *(ckt->CKTrhsOld+here->BSIMbNode) -
                *(ckt->CKTrhsOld+here->BSIMsNodePrime));
            vgs = model->BSIMtype * ( 
                *(ckt->CKTrhsOld+here->BSIMgNode) -
                *(ckt->CKTrhsOld+here->BSIMsNodePrime));
            vds = model->BSIMtype * ( 
                *(ckt->CKTrhsOld+here->BSIMdNodePrime) -
                *(ckt->CKTrhsOld+here->BSIMsNodePrime));
            vbd=vbs-vds;
            vgd=vgs-vds;
            vgdo = *(ckt->CKTstate0 + here->BSIMvgs) - 
                *(ckt->CKTstate0 + here->BSIMvds);
            delvbs = vbs - *(ckt->CKTstate0 + here->BSIMvbs);
            delvbd = vbd - *(ckt->CKTstate0 + here->BSIMvbd);
            delvgs = vgs - *(ckt->CKTstate0 + here->BSIMvgs);
            delvds = vds - *(ckt->CKTstate0 + here->BSIMvds);
            delvgd = vgd-vgdo;

            if (here->BSIMmode >= 0) {
                cdhat=
                    *(ckt->CKTstate0 + here->BSIMcd) -
                    *(ckt->CKTstate0 + here->BSIMgbd) * delvbd +
                    *(ckt->CKTstate0 + here->BSIMgmbs) * delvbs +
                    *(ckt->CKTstate0 + here->BSIMgm) * delvgs + 
                    *(ckt->CKTstate0 + here->BSIMgds) * delvds ;
            } else {
                cdhat=
                    *(ckt->CKTstate0 + here->BSIMcd) -
                    ( *(ckt->CKTstate0 + here->BSIMgbd) -
                      *(ckt->CKTstate0 + here->BSIMgmbs)) * delvbd -
                    *(ckt->CKTstate0 + here->BSIMgm) * delvgd +
                    *(ckt->CKTstate0 + here->BSIMgds) * delvds;
            }
            cbhat=
                *(ckt->CKTstate0 + here->BSIMcbs) +
                *(ckt->CKTstate0 + here->BSIMcbd) +
                *(ckt->CKTstate0 + here->BSIMgbd) * delvbd +
                *(ckt->CKTstate0 + here->BSIMgbs) * delvbs ;

            cd = *(ckt->CKTstate0 + here->BSIMcd);
            cbs = *(ckt->CKTstate0 + here->BSIMcbs);
            cbd = *(ckt->CKTstate0 + here->BSIMcbd);
            /*
             *  check convergence
             */
            if ( (here->BSIMoff == 0)  || (!(ckt->CKTmode & MODEINITFIX)) ){
                tol=ckt->CKTreltol*MAX(FABS(cdhat),FABS(cd))+ckt->CKTabstol;
                if (FABS(cdhat-cd) >= tol) { 
/* gtri - begin - wbk - report conv prob */
                    if(ckt->enh->conv_debug.report_conv_probs) {
                        ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                            (char *) here->BSIMname,
                                            "");
                    }
/* gtri - end - wbk - report conv prob */
                    ckt->CKTnoncon++;
                    return(OK);
                } 
                tol=ckt->CKTreltol*MAX(FABS(cbhat),FABS(cbs+cbd))+
                    ckt->CKTabstol;
                if (FABS(cbhat-(cbs+cbd)) > tol) {
/* gtri - begin - wbk - report conv prob */
                    if(ckt->enh->conv_debug.report_conv_probs) {
                        ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                            (char *) here->BSIMname,
                                            "");
                    }
/* gtri - end - wbk - report conv prob */
                    ckt->CKTnoncon++;
                    return(OK);
                }
            }
        }
    }
    return(OK);
}

