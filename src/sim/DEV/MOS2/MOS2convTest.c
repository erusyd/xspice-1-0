/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "MOS2defs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("MOS2convTest.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:51 $")

int
MOS2convTest(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register MOS2model *model = (MOS2model *)inModel;
    register MOS2instance *here;
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

    for( ; model != NULL; model = model->MOS2nextModel) {
        for(here = model->MOS2instances; here!= NULL;
                here = here->MOS2nextInstance) {
        
            vbs = model->MOS2type * ( 
                *(ckt->CKTrhs+here->MOS2bNode) -
                *(ckt->CKTrhs+here->MOS2sNodePrime));
            vgs = model->MOS2type * ( 
                *(ckt->CKTrhs+here->MOS2gNode) -
                *(ckt->CKTrhs+here->MOS2sNodePrime));
            vds = model->MOS2type * ( 
                *(ckt->CKTrhs+here->MOS2dNodePrime) -
                *(ckt->CKTrhs+here->MOS2sNodePrime));
            vbd=vbs-vds;
            vgd=vgs-vds;
            vgdo = *(ckt->CKTstate0 + here->MOS2vgs) -
                *(ckt->CKTstate0 + here->MOS2vds);
            delvbs = vbs - *(ckt->CKTstate0 + here->MOS2vbs);
            delvbd = vbd - *(ckt->CKTstate0 + here->MOS2vbd);
            delvgs = vgs - *(ckt->CKTstate0 + here->MOS2vgs);
            delvds = vds - *(ckt->CKTstate0 + here->MOS2vds);
            delvgd = vgd-vgdo;

            /* these are needed for convergence testing */

            if (here->MOS2mode >= 0) {
                cdhat=
                    here->MOS2cd-
                    here->MOS2gbd * delvbd +
                    here->MOS2gmbs * delvbs +
                    here->MOS2gm * delvgs + 
                    here->MOS2gds * delvds ;
            } else {
                cdhat=
                    here->MOS2cd -
                    ( here->MOS2gbd -
                    here->MOS2gmbs) * delvbd -
                    here->MOS2gm * delvgd + 
                    here->MOS2gds * delvds ;
            }
            cbhat=
                here->MOS2cbs +
                here->MOS2cbd +
                here->MOS2gbd * delvbd +
                here->MOS2gbs * delvbs ;
            /*
             *  check convergence
             */
            tol=ckt->CKTreltol*MAX(FABS(cdhat),FABS(here->MOS2cd))+
                    ckt->CKTabstol;
            if (FABS(cdhat-here->MOS2cd) >= tol) { 
/* gtri - begin - wbk - report conv prob */
                if(ckt->enh->conv_debug.report_conv_probs) {
                    ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                        (char *) here->MOS2name,
                                        "");
                }
/* gtri - end - wbk - report conv prob */
                ckt->CKTnoncon++;
                return(OK); /* no reason to continue, we haven't converged */
            } else {
                tol=ckt->CKTreltol*
                        MAX(FABS(cbhat),FABS(here->MOS2cbs+here->MOS2cbd))+ 
                        ckt->CKTabstol;
                if (FABS(cbhat-(here->MOS2cbs+here->MOS2cbd)) > tol) {
/* gtri - begin - wbk - report conv prob */
                    if(ckt->enh->conv_debug.report_conv_probs) {
                        ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                            (char *) here->MOS2name,
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
