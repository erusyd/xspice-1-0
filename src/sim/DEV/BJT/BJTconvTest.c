/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*
 * This routine performs the device convergence test for
 * BJTs in the circuit.
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "BJTdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("BJTConvTest.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:37 $")

int
BJTconvTest(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;

{
    register BJTinstance *here;
    register BJTmodel *model = (BJTmodel *) inModel;
    double tol;
    double cc;
    double cchat;
    double cb;
    double cbhat;
    double vbe;
    double vbc;
    double delvbe;
    double delvbc;



    for( ; model != NULL; model = model->BJTnextModel) {
        for(here=model->BJTinstances;here!=NULL;here = here->BJTnextInstance){
            vbe=model->BJTtype*(
                    *(ckt->CKTrhsOld+here->BJTbasePrimeNode)-
                    *(ckt->CKTrhsOld+here->BJTemitPrimeNode));
            vbc=model->BJTtype*(
                    *(ckt->CKTrhsOld+here->BJTbasePrimeNode)-
                    *(ckt->CKTrhsOld+here->BJTcolPrimeNode));
            delvbe=vbe- *(ckt->CKTstate0 + here->BJTvbe);
            delvbc=vbc- *(ckt->CKTstate0 + here->BJTvbc);
            cchat= *(ckt->CKTstate0 + here->BJTcc)+(*(ckt->CKTstate0 + 
                    here->BJTgm)+ *(ckt->CKTstate0 + here->BJTgo))*delvbe-
                    (*(ckt->CKTstate0 + here->BJTgo)+*(ckt->CKTstate0 +
                    here->BJTgmu))*delvbc;
            cbhat= *(ckt->CKTstate0 + here->BJTcb)+ *(ckt->CKTstate0 + 
                    here->BJTgpi)*delvbe+ *(ckt->CKTstate0 + here->BJTgmu)*
                    delvbc;
            cc = *(ckt->CKTstate0 + here->BJTcc);
            cb = *(ckt->CKTstate0 + here->BJTcb);
            /*
             *   check convergence
             */
            tol=ckt->CKTreltol*MAX(FABS(cchat),FABS(cc))+ckt->CKTabstol;
            if (FABS(cchat-cc) > tol) {
/* gtri - begin - wbk - report conv prob */
                if(ckt->enh->conv_debug.report_conv_probs) {
                    ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                        (char *) here->BJTname,
                                        "");
                }
/* gtri - end - wbk - report conv prob */
                ckt->CKTnoncon++;
                return(OK); /* no reason to continue - we've failed... */
            } else {
                tol=ckt->CKTreltol*MAX(FABS(cbhat),FABS(cb))+
                    ckt->CKTabstol;
                if (FABS(cbhat-cb) > tol) {
/* gtri - begin - wbk - report conv prob */
                    if(ckt->enh->conv_debug.report_conv_probs) {
                        ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                            (char *) here->BJTname,
                                            "");
                    }
/* gtri - end - wbk - report conv prob */
                    ckt->CKTnoncon++;
                    return(OK); /* no reason to continue - we've failed... */
                }
            }
        }
    }
    return(OK);
}
