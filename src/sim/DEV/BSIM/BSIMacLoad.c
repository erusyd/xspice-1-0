/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "BSIMdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMacLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:00 $")

int
BSIMacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register BSIMmodel *model = (BSIMmodel*)inModel;
    register BSIMinstance *here;
    int xnrm;
    int xrev;
    double gdpr;
    double gspr;
    double gm;
    double gds;
    double gmbs;
    double gbd;
    double gbs;
    double capbd;
    double capbs;
    double xcggb;
    double xcgdb;
    double xcgsb;
    double xcbgb;
    double xcbdb;
    double xcbsb;
    double xcddb;
    double xcssb;
    double xcdgb;
    double xcsgb;
    double xcdsb;
    double xcsdb;
    double cggb;
    double cgdb;
    double cgsb;
    double cbgb;
    double cbdb;
    double cbsb;
    double cddb;
    double cdgb;
    double cdsb;
    double omega; /* angular fequency of the signal */

    omega = ckt->CKTomega;
    for( ; model != NULL; model = model->BSIMnextModel) {
        for(here = model->BSIMinstances; here!= NULL;
                here = here->BSIMnextInstance) {
        
            if (here->BSIMmode >= 0) {
                xnrm=1;
                xrev=0;
            } else {
                xnrm=0;
                xrev=1;
            }
            gdpr=here->BSIMdrainConductance;
            gspr=here->BSIMsourceConductance;
            gm= *(ckt->CKTstate0 + here->BSIMgm);
            gds= *(ckt->CKTstate0 + here->BSIMgds);
            gmbs= *(ckt->CKTstate0 + here->BSIMgmbs);
            gbd= *(ckt->CKTstate0 + here->BSIMgbd);
            gbs= *(ckt->CKTstate0 + here->BSIMgbs);
            capbd= *(ckt->CKTstate0 + here->BSIMcapbd);
            capbs= *(ckt->CKTstate0 + here->BSIMcapbs);
            /*
             *    charge oriented model parameters
             */

            cggb = *(ckt->CKTstate0 + here->BSIMcggb);
            cgsb = *(ckt->CKTstate0 + here->BSIMcgsb);
            cgdb = *(ckt->CKTstate0 + here->BSIMcgdb);

            cbgb = *(ckt->CKTstate0 + here->BSIMcbgb);
            cbsb = *(ckt->CKTstate0 + here->BSIMcbsb);
            cbdb = *(ckt->CKTstate0 + here->BSIMcbdb);

            cdgb = *(ckt->CKTstate0 + here->BSIMcdgb);
            cdsb = *(ckt->CKTstate0 + here->BSIMcdsb);
            cddb = *(ckt->CKTstate0 + here->BSIMcddb);

            xcdgb = (cdgb - here->BSIMGDoverlapCap) * omega;
            xcddb = (cddb + capbd + here->BSIMGDoverlapCap) * omega;
            xcdsb = cdsb * omega;
            xcsgb = -(cggb + cbgb + cdgb + here->BSIMGSoverlapCap ) * omega;
            xcsdb = -(cgdb + cbdb + cddb) * omega;
            xcssb = (capbs + here->BSIMGSoverlapCap - (cgsb+cbsb+cdsb)) * omega;
            xcggb = (cggb + here->BSIMGDoverlapCap + here->BSIMGSoverlapCap + 
                    here->BSIMGBoverlapCap) * omega;
            xcgdb = (cgdb - here->BSIMGDoverlapCap ) * omega;
            xcgsb = (cgsb - here->BSIMGSoverlapCap) * omega;
            xcbgb = (cbgb - here->BSIMGBoverlapCap) * omega;
            xcbdb = (cbdb - capbd ) * omega;
            xcbsb = (cbsb - capbs ) * omega;


            *(here->BSIMGgPtr +1) += xcggb;
            *(here->BSIMBbPtr +1) += -xcbgb-xcbdb-xcbsb;
            *(here->BSIMDPdpPtr +1) += xcddb;
            *(here->BSIMSPspPtr +1) += xcssb;
            *(here->BSIMGbPtr +1) += -xcggb-xcgdb-xcgsb;
            *(here->BSIMGdpPtr +1) += xcgdb;
            *(here->BSIMGspPtr +1) += xcgsb;
            *(here->BSIMBgPtr +1) += xcbgb;
            *(here->BSIMBdpPtr +1) += xcbdb;
            *(here->BSIMBspPtr +1) += xcbsb;
            *(here->BSIMDPgPtr +1) += xcdgb;
            *(here->BSIMDPbPtr +1) += -xcdgb-xcddb-xcdsb;
            *(here->BSIMDPspPtr +1) += xcdsb;
            *(here->BSIMSPgPtr +1) += xcsgb;
            *(here->BSIMSPbPtr +1) += -xcsgb-xcsdb-xcssb;
            *(here->BSIMSPdpPtr +1) += xcsdb;
            *(here->BSIMDdPtr) += gdpr;
            *(here->BSIMSsPtr) += gspr;
            *(here->BSIMBbPtr) += gbd+gbs;
            *(here->BSIMDPdpPtr) += gdpr+gds+gbd+xrev*(gm+gmbs);
            *(here->BSIMSPspPtr) += gspr+gds+gbs+xnrm*(gm+gmbs);
            *(here->BSIMDdpPtr) -= gdpr;
            *(here->BSIMSspPtr) -= gspr;
            *(here->BSIMBdpPtr) -= gbd;
            *(here->BSIMBspPtr) -= gbs;
            *(here->BSIMDPdPtr) -= gdpr;
            *(here->BSIMDPgPtr) += (xnrm-xrev)*gm;
            *(here->BSIMDPbPtr) += -gbd+(xnrm-xrev)*gmbs;
            *(here->BSIMDPspPtr) += -gds-xnrm*(gm+gmbs);
            *(here->BSIMSPgPtr) += -(xnrm-xrev)*gm;
            *(here->BSIMSPsPtr) -= gspr;
            *(here->BSIMSPbPtr) += -gbs-(xnrm-xrev)*gmbs;
            *(here->BSIMSPdpPtr) += -gds-xrev*(gm+gmbs);

        }
    }
return(OK);
}


