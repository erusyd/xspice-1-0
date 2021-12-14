/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "MOS1defs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("MOS1convTest.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:21 $")

int
MOS1convTest(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register MOS1model *model = (MOS1model*)inModel;
    register MOS1instance *here;
    double delvbs;
    double delvbd;
    double delvgs;
    double delvds;
    double delvgd;
    double cbhat;
    double cdhat;
    double vbs;
    double vbd;
    double vgs;
    double vds;
    double vgd;
    double vgdo;
    double tol;

    for( ; model != NULL; model = model->MOS1nextModel) {
        for(here = model->MOS1instances; here!= NULL;
                here = here->MOS1nextInstance) {
        
            vbs = model->MOS1type * ( 
                *(ckt->CKTrhs+here->MOS1bNode) -
                *(ckt->CKTrhs+here->MOS1sNodePrime));
            vgs = model->MOS1type * ( 
                *(ckt->CKTrhs+here->MOS1gNode) -
                *(ckt->CKTrhs+here->MOS1sNodePrime));
            vds = model->MOS1type * ( 
                *(ckt->CKTrhs+here->MOS1dNodePrime) -
                *(ckt->CKTrhs+here->MOS1sNodePrime));
            vbd=vbs-vds;
            vgd=vgs-vds;
            vgdo = *(ckt->CKTstate0 + here->MOS1vgs) -
                *(ckt->CKTstate0 + here->MOS1vds);
            delvbs = vbs - *(ckt->CKTstate0 + here->MOS1vbs);
            delvbd = vbd - *(ckt->CKTstate0 + here->MOS1vbd);
            delvgs = vgs - *(ckt->CKTstate0 + here->MOS1vgs);
            delvds = vds - *(ckt->CKTstate0 + here->MOS1vds);
            delvgd = vgd-vgdo;

            /* these are needed for convergence testing */

            if (here->MOS1mode >= 0) {
                cdhat=
                    here->MOS1cd-
                    here->MOS1gbd * delvbd +
                    here->MOS1gmbs * delvbs +
                    here->MOS1gm * delvgs + 
                    here->MOS1gds * delvds ;
            } else {
                cdhat=
                    here->MOS1cd -
                    ( here->MOS1gbd -
                    here->MOS1gmbs) * delvbd -
                    here->MOS1gm * delvgd + 
                    here->MOS1gds * delvds ;
            }
            cbhat=
                here->MOS1cbs +
                here->MOS1cbd +
                here->MOS1gbd * delvbd +
                here->MOS1gbs * delvbs ;
            /*
             *  check convergence
             */
            tol=ckt->CKTreltol*MAX(FABS(cdhat),FABS(here->MOS1cd))+
                    ckt->CKTabstol;
            if (FABS(cdhat-here->MOS1cd) >= tol) { 
/* gtri - begin - wbk - report conv prob */
                if(ckt->enh->conv_debug.report_conv_probs) {
                    ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                        (char *) here->MOS1name,
                                        "");
                }
/* gtri - end - wbk - report conv prob */
                ckt->CKTnoncon++;
                return(OK); /* no reason to continue, we haven't converged */
            } else {
                tol=ckt->CKTreltol*
                        MAX(FABS(cbhat),FABS(here->MOS1cbs+here->MOS1cbd))+
                        ckt->CKTabstol;
                if (FABS(cbhat-(here->MOS1cbs+here->MOS1cbd)) > tol) {
/* gtri - begin - wbk - report conv prob */
                    if(ckt->enh->conv_debug.report_conv_probs) {
                        ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                            (char *) here->MOS1name,
                                            "");
                    }
/* gtri - end - wbk - report conv prob */
                    ckt->CKTnoncon++;
                    return(OK); /* no reason to continue, we haven't converged*/
                }
            }
        }
    }
    return(OK);
}
