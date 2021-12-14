/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "complex.h"
#include "SPerror.h"
#include "BSIMdefs.h"
#include "suffix.h"

RCSID("BSIMpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:11 $")

int
BSIMpzLoad(inModel,ckt,s)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    register SPcomplex *s;
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

            xcdgb = (cdgb - here->BSIMGDoverlapCap) ;
            xcddb = (cddb + capbd + here->BSIMGDoverlapCap) ;
            xcdsb = cdsb ;
            xcsgb = -(cggb + cbgb + cdgb + here->BSIMGSoverlapCap ) ;
            xcsdb = -(cgdb + cbdb + cddb) ;
            xcssb = (capbs + here->BSIMGSoverlapCap - (cgsb+cbsb+cdsb)) ;
            xcggb = (cggb + here->BSIMGDoverlapCap + here->BSIMGSoverlapCap + 
                    here->BSIMGBoverlapCap) ;
            xcgdb = (cgdb - here->BSIMGDoverlapCap ) ;
            xcgsb = (cgsb - here->BSIMGSoverlapCap) ;
            xcbgb = (cbgb - here->BSIMGBoverlapCap) ;
            xcbdb = (cbdb - capbd ) ;
            xcbsb = (cbsb - capbs ) ;


            *(here->BSIMGgPtr   ) += xcggb * s->real;
            *(here->BSIMGgPtr +1) += xcggb * s->imag;
            *(here->BSIMBbPtr   ) += (-xcbgb-xcbdb-xcbsb) * s->real;
            *(here->BSIMBbPtr +1) += (-xcbgb-xcbdb-xcbsb) * s->imag;
            *(here->BSIMDPdpPtr   ) += xcddb * s->real;
            *(here->BSIMDPdpPtr +1) += xcddb * s->imag;
            *(here->BSIMSPspPtr   ) += xcssb * s->real;
            *(here->BSIMSPspPtr +1) += xcssb * s->imag;
            *(here->BSIMGbPtr   ) += (-xcggb-xcgdb-xcgsb) * s->real;
            *(here->BSIMGbPtr +1) += (-xcggb-xcgdb-xcgsb) * s->imag;
            *(here->BSIMGdpPtr   ) += xcgdb * s->real;
            *(here->BSIMGdpPtr +1) += xcgdb * s->imag;
            *(here->BSIMGspPtr   ) += xcgsb * s->real;
            *(here->BSIMGspPtr +1) += xcgsb * s->imag;
            *(here->BSIMBgPtr   ) += xcbgb * s->real;
            *(here->BSIMBgPtr +1) += xcbgb * s->imag;
            *(here->BSIMBdpPtr   ) += xcbdb * s->real;
            *(here->BSIMBdpPtr +1) += xcbdb * s->imag;
            *(here->BSIMBspPtr   ) += xcbsb * s->real;
            *(here->BSIMBspPtr +1) += xcbsb * s->imag;
            *(here->BSIMDPgPtr   ) += xcdgb * s->real;
            *(here->BSIMDPgPtr +1) += xcdgb * s->imag;
            *(here->BSIMDPbPtr   ) += (-xcdgb-xcddb-xcdsb) * s->real;
            *(here->BSIMDPbPtr +1) += (-xcdgb-xcddb-xcdsb) * s->imag;
            *(here->BSIMDPspPtr   ) += xcdsb * s->real;
            *(here->BSIMDPspPtr +1) += xcdsb * s->imag;
            *(here->BSIMSPgPtr   ) += xcsgb * s->real;
            *(here->BSIMSPgPtr +1) += xcsgb * s->imag;
            *(here->BSIMSPbPtr   ) += (-xcsgb-xcsdb-xcssb) * s->real;
            *(here->BSIMSPbPtr +1) += (-xcsgb-xcsdb-xcssb) * s->imag;
            *(here->BSIMSPdpPtr   ) += xcsdb * s->real;
            *(here->BSIMSPdpPtr +1) += xcsdb * s->imag;
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
