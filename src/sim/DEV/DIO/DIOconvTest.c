/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef LINT
static char rcsid[] =
    "DIOconvTest.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:34 $";
#endif

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "DEVdefs.h"
#include "CKTdefs.h"
#include "DIOdefs.h"
#include "CONST.h"
#include "TRANdefs.h"
#include <math.h>
#include "SPerror.h"
#include "suffix.h"

int
DIOconvTest(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /* Check the devices for convergence
         */
{
    register DIOmodel *model = (DIOmodel*)inModel;
    register DIOinstance *here;
    double delvd,vd,cdhat,cd;
    double tol;
    /*  loop through all the diode models */
    for( ; model != NULL; model = model->DIOnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->DIOinstances; here != NULL ;
                here=here->DIOnextInstance) {
                
            /*  
             *   initialization 
             */

            vd = *(ckt->CKTrhsOld+here->DIOposPrimeNode)-
                    *(ckt->CKTrhsOld + here->DIOnegNode);

            delvd=vd- *(ckt->CKTstate0 + here->DIOvoltage);
            cdhat= *(ckt->CKTstate0 + here->DIOcurrent) + 
                    *(ckt->CKTstate0 + here->DIOconduct) * delvd;

            cd= *(ckt->CKTstate0 + here->DIOcurrent);

            /*
             *   check convergence
             */
            tol=ckt->CKTreltol*
                    MAX(FABS(cdhat),FABS(cd))+ckt->CKTabstol;
            if (FABS(cdhat-cd) > tol) {
/* gtri - begin - wbk - report conv prob */
                if(ckt->enh->conv_debug.report_conv_probs) {
                    ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                        (char *) here->DIOname,
                                        "");
                }
/* gtri - end - wbk - report conv prob */
                ckt->CKTnoncon++;
                return(OK); /* don't need to check any more device */
            }
        }
    }
    return(OK);
}
