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
#include "SPerror.h"
#include "DEVdefs.h"
#include "suffix.h"

RCSID("BSIMload.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:06 $")

int
BSIMload(inModel,ckt)

    GENmodel *inModel;
    register CKTcircuit *ckt;

        /* actually load the current value into the 
         * sparse matrix previously provided 
         */
{
    register BSIMmodel *model = (BSIMmodel*)inModel;
    register BSIMinstance *here;
    double DrainSatCurrent;
    double EffectiveLength;
    double GateBulkOverlapCap;
    double GateDrainOverlapCap;
    double GateSourceOverlapCap;
    double SourceSatCurrent;
    double DrainArea;
    double SourceArea;
    double DrainPerimeter;
    double SourcePerimeter;
    double arg;
    double capbd;
    double capbs;
    double cbd;
    double cbhat;
    double cbs;
    double cd;
    double cdrain;
    double cdhat;
    double cdreq;
    double ceq;
    double ceqbd;
    double ceqbs;
    double ceqqb;
    double ceqqd;
    double ceqqg;
    double czbd;
    double czbdsw;
    double czbs;
    double czbssw;
    double delvbd;
    double delvbs;
    double delvds;
    double delvgd;
    double delvgs;
    double evbd;
    double evbs;
    double gbd;
    double gbs;
    double gcbdb;
    double gcbgb;
    double gcbsb;
    double gcddb;
    double gcdgb;
    double gcdsb;
    double gcgdb;
    double gcggb;
    double gcgsb;
    double gcsdb;
    double gcsgb;
    double gcssb;
    double gds;
    double geq;
    double gm;
    double gmbs;
    double sarg;
    double sargsw;
    double tol;
    double vbd;
    double vbs;
    double vcrit;
    double vds;
    double vdsat;
    double vgb;
    double vgd;
    double vgdo;
    double vgs;
    double von;
    double xfact;
    double xnrm;
    double xrev;
    int Check;
    double cgdb;
    double cgsb;
    double cbdb;
    double cdgb;
    double cddb;
    double cdsb;
    double cggb;
    double cbgb;
    double cbsb;
    double csgb;
    double cssb;
    double csdb;
    double PhiB;
    double PhiBSW;
    double MJ;
    double MJSW;
    double argsw;
    double qgate;
    double qbulk;
    double qdrn;
    double qsrc;
    double cqgate;
    double cqbulk;
    double cqdrn;
    double vt0;
    int    ByPass;
#ifndef NOBYPASS
    double tempv;
#endif /*NOBYPASS*/
    int error;


    /*  loop through all the BSIM device models */
    for( ; model != NULL; model = model->BSIMnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->BSIMinstances; here != NULL ;
                here=here->BSIMnextInstance) {
            EffectiveLength=here->BSIMl - model->BSIMdeltaL * 1.e-6;/* m */
            DrainArea = here->BSIMdrainArea;
            SourceArea = here->BSIMsourceArea;
            DrainPerimeter = here->BSIMdrainPerimeter;
            SourcePerimeter = here->BSIMsourcePerimeter;
            if( (DrainSatCurrent=DrainArea*model->BSIMjctSatCurDensity) 
                    < 1e-15){
                DrainSatCurrent = 1.0e-15;
            }
            if( (SourceSatCurrent=SourceArea*model->BSIMjctSatCurDensity)
                    <1.0e-15){
                SourceSatCurrent = 1.0e-15;
            }
            GateSourceOverlapCap = model->BSIMgateSourceOverlapCap *here->BSIMw;
            GateDrainOverlapCap = model->BSIMgateDrainOverlapCap * here->BSIMw;
            GateBulkOverlapCap = model->BSIMgateBulkOverlapCap *EffectiveLength;
            von = model->BSIMtype * here->BSIMvon;
            vdsat = model->BSIMtype * here->BSIMvdsat;
            vt0 = model->BSIMtype * here->BSIMvt0;

            Check=1;
            ByPass = 0;
            if((ckt->CKTmode & MODEINITSMSIG)) {
                vbs= *(ckt->CKTstate0 + here->BSIMvbs);
                vgs= *(ckt->CKTstate0 + here->BSIMvgs);
                vds= *(ckt->CKTstate0 + here->BSIMvds);
            } else if ((ckt->CKTmode & MODEINITTRAN)) {
                vbs= *(ckt->CKTstate1 + here->BSIMvbs);
                vgs= *(ckt->CKTstate1 + here->BSIMvgs);
                vds= *(ckt->CKTstate1 + here->BSIMvds);
            } else if((ckt->CKTmode & MODEINITJCT) && !here->BSIMoff) {
                vds= model->BSIMtype * here->BSIMicVDS;
                vgs= model->BSIMtype * here->BSIMicVGS;
                vbs= model->BSIMtype * here->BSIMicVBS;
                if((vds==0) && (vgs==0) && (vbs==0) && 
                        ((ckt->CKTmode & 
                        (MODETRAN|MODEAC|MODEDCOP|MODEDCTRANCURVE)) ||
                        (!(ckt->CKTmode & MODEUIC)))) {
                    vbs = -1;
                    vgs = vt0;
                    vds = 0;
                }
            } else if((ckt->CKTmode & (MODEINITJCT | MODEINITFIX) ) && 
                    (here->BSIMoff)) {
                vbs=vgs=vds=0;
            } else {
#ifndef PREDICTOR
                if((ckt->CKTmode & MODEINITPRED)) {
                    xfact=ckt->CKTdelta/ckt->CKTdeltaOld[1];
                    *(ckt->CKTstate0 + here->BSIMvbs) = 
                            *(ckt->CKTstate1 + here->BSIMvbs);
                    vbs = (1+xfact)* (*(ckt->CKTstate1 + here->BSIMvbs))
                            -(xfact * (*(ckt->CKTstate2 + here->BSIMvbs)));
                    *(ckt->CKTstate0 + here->BSIMvgs) = 
                            *(ckt->CKTstate1 + here->BSIMvgs);
                    vgs = (1+xfact)* (*(ckt->CKTstate1 + here->BSIMvgs))
                            -(xfact * (*(ckt->CKTstate2 + here->BSIMvgs)));
                    *(ckt->CKTstate0 + here->BSIMvds) = 
                            *(ckt->CKTstate1 + here->BSIMvds);
                    vds = (1+xfact)* (*(ckt->CKTstate1 + here->BSIMvds))
                            -(xfact * (*(ckt->CKTstate2 + here->BSIMvds)));
                    *(ckt->CKTstate0 + here->BSIMvbd) = 
                            *(ckt->CKTstate0 + here->BSIMvbs)-
                            *(ckt->CKTstate0 + here->BSIMvds);
                    *(ckt->CKTstate0 + here->BSIMcd) = 
                            *(ckt->CKTstate1 + here->BSIMcd);
                    *(ckt->CKTstate0 + here->BSIMcbs) = 
                            *(ckt->CKTstate1 + here->BSIMcbs);
                    *(ckt->CKTstate0 + here->BSIMcbd) = 
                            *(ckt->CKTstate1 + here->BSIMcbd);
                    *(ckt->CKTstate0 + here->BSIMgm) = 
                            *(ckt->CKTstate1 + here->BSIMgm);
                    *(ckt->CKTstate0 + here->BSIMgds) = 
                            *(ckt->CKTstate1 + here->BSIMgds);
                    *(ckt->CKTstate0 + here->BSIMgmbs) = 
                            *(ckt->CKTstate1 + here->BSIMgmbs);
                    *(ckt->CKTstate0 + here->BSIMgbd) = 
                            *(ckt->CKTstate1 + here->BSIMgbd);
                    *(ckt->CKTstate0 + here->BSIMgbs) = 
                            *(ckt->CKTstate1 + here->BSIMgbs);
                    *(ckt->CKTstate0 + here->BSIMcggb) = 
                            *(ckt->CKTstate1 + here->BSIMcggb);
                    *(ckt->CKTstate0 + here->BSIMcbgb) = 
                            *(ckt->CKTstate1 + here->BSIMcbgb);
                    *(ckt->CKTstate0 + here->BSIMcbsb) = 
                            *(ckt->CKTstate1 + here->BSIMcbsb);
                    *(ckt->CKTstate0 + here->BSIMcgdb) = 
                            *(ckt->CKTstate1 + here->BSIMcgdb);
                    *(ckt->CKTstate0 + here->BSIMcgsb) = 
                            *(ckt->CKTstate1 + here->BSIMcgsb);
                    *(ckt->CKTstate0 + here->BSIMcbdb) = 
                            *(ckt->CKTstate1 + here->BSIMcbdb);
                    *(ckt->CKTstate0 + here->BSIMcdgb) = 
                            *(ckt->CKTstate1 + here->BSIMcdgb);
                    *(ckt->CKTstate0 + here->BSIMcddb) = 
                            *(ckt->CKTstate1 + here->BSIMcddb);
                    *(ckt->CKTstate0 + here->BSIMcdsb) = 
                            *(ckt->CKTstate1 + here->BSIMcdsb);
                } else {
#endif /* PREDICTOR */
                    vbs = model->BSIMtype * ( 
                        *(ckt->CKTrhsOld+here->BSIMbNode) -
                        *(ckt->CKTrhsOld+here->BSIMsNodePrime));
                    vgs = model->BSIMtype * ( 
                        *(ckt->CKTrhsOld+here->BSIMgNode) -
                        *(ckt->CKTrhsOld+here->BSIMsNodePrime));
                    vds = model->BSIMtype * ( 
                        *(ckt->CKTrhsOld+here->BSIMdNodePrime) -
                        *(ckt->CKTrhsOld+here->BSIMsNodePrime));
#ifndef PREDICTOR
                }
#endif /* PREDICTOR */
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

#ifndef NOBYPASS
                    /* now lets see if we can bypass (ugh) */

                /* following should be one big if connected by && all over
                 * the place, but some C compilers can't handle that, so
                 * we split it up here to let them digest it in stages
                 */
                tempv = MAX(FABS(cbhat),FABS(*(ckt->CKTstate0 + here->BSIMcbs)
                        + *(ckt->CKTstate0 + here->BSIMcbd)))+ckt->CKTabstol;
                if((!(ckt->CKTmode & MODEINITPRED)) && (ckt->CKTbypass) )
                if( (FABS(delvbs) < (ckt->CKTreltol * MAX(FABS(vbs),
                        FABS(*(ckt->CKTstate0+here->BSIMvbs)))+
                        ckt->CKTvoltTol)) )
                if ( (FABS(delvbd) < (ckt->CKTreltol * MAX(FABS(vbd),
                        FABS(*(ckt->CKTstate0+here->BSIMvbd)))+
                        ckt->CKTvoltTol)) )
                if( (FABS(delvgs) < (ckt->CKTreltol * MAX(FABS(vgs),
                        FABS(*(ckt->CKTstate0+here->BSIMvgs)))+
                        ckt->CKTvoltTol)))
                if ( (FABS(delvds) < (ckt->CKTreltol * MAX(FABS(vds),
                        FABS(*(ckt->CKTstate0+here->BSIMvds)))+
                        ckt->CKTvoltTol)) )
                if( (FABS(cdhat- *(ckt->CKTstate0 + here->BSIMcd)) <
                        ckt->CKTreltol * MAX(FABS(cdhat),FABS(*(ckt->CKTstate0 +
                        here->BSIMcd))) + ckt->CKTabstol) )
                if ( (FABS(cbhat-(*(ckt->CKTstate0 + here->BSIMcbs) +
                        *(ckt->CKTstate0 + here->BSIMcbd))) < ckt->CKTreltol *
                        tempv)) {
                    /* bypass code */
                    vbs = *(ckt->CKTstate0 + here->BSIMvbs);
                    vbd = *(ckt->CKTstate0 + here->BSIMvbd);
                    vgs = *(ckt->CKTstate0 + here->BSIMvgs);
                    vds = *(ckt->CKTstate0 + here->BSIMvds);
                    vgd = vgs - vds;
                    vgb = vgs - vbs;
                    cd = *(ckt->CKTstate0 + here->BSIMcd);
                    cbs = *(ckt->CKTstate0 + here->BSIMcbs);
                    cbd = *(ckt->CKTstate0 + here->BSIMcbd);
                    cdrain = here->BSIMmode * (cd + cbd);
                    gm = *(ckt->CKTstate0 + here->BSIMgm);
                    gds = *(ckt->CKTstate0 + here->BSIMgds);
                    gmbs = *(ckt->CKTstate0 + here->BSIMgmbs);
                    gbd = *(ckt->CKTstate0 + here->BSIMgbd);
                    gbs = *(ckt->CKTstate0 + here->BSIMgbs);
                    if((ckt->CKTmode & (MODETRAN | MODEAC)) || 
                            ((ckt->CKTmode & MODETRANOP) && 
                            (ckt->CKTmode & MODEUIC))) {
                        cggb = *(ckt->CKTstate0 + here->BSIMcggb);
                        cgdb = *(ckt->CKTstate0 + here->BSIMcgdb);
                        cgsb = *(ckt->CKTstate0 + here->BSIMcgsb);
                        cbgb = *(ckt->CKTstate0 + here->BSIMcbgb);
                        cbdb = *(ckt->CKTstate0 + here->BSIMcbdb);
                        cbsb = *(ckt->CKTstate0 + here->BSIMcbsb);
                        cdgb = *(ckt->CKTstate0 + here->BSIMcdgb);
                        cddb = *(ckt->CKTstate0 + here->BSIMcddb);
                        cdsb = *(ckt->CKTstate0 + here->BSIMcdsb);
                        capbs = *(ckt->CKTstate0 + here->BSIMcapbs);
                        capbd = *(ckt->CKTstate0 + here->BSIMcapbd);
                        ByPass = 1;
                        goto line755;
                    } else {
                        goto line850;
                    }
                }
#endif /*NOBYPASS*/

                von = model->BSIMtype * here->BSIMvon;
                if(*(ckt->CKTstate0 + here->BSIMvds) >=0) {
                    vgs = DEVfetlim(vgs,*(ckt->CKTstate0 + here->BSIMvgs)
                            ,von);
                    vds = vgs - vgd;
                    vds = DEVlimvds(vds,*(ckt->CKTstate0 + here->BSIMvds));
                    vgd = vgs - vds;
                } else {
                    vgd = DEVfetlim(vgd,vgdo,von);
                    vds = vgs - vgd;
                    vds = -DEVlimvds(-vds,-(*(ckt->CKTstate0 + here->BSIMvds)));
                    vgs = vgd + vds;
                }
                if(vds >= 0) {
                    vcrit =CONSTvt0*log(CONSTvt0/(CONSTroot2*SourceSatCurrent));
                    vbs = DEVpnjlim(vbs,*(ckt->CKTstate0 + here->BSIMvbs),
                            CONSTvt0,vcrit,&Check); /* BSIM test */
                    vbd = vbs-vds;
                } else {
                    vcrit = CONSTvt0*log(CONSTvt0/(CONSTroot2*DrainSatCurrent));
                    vbd = DEVpnjlim(vbd,*(ckt->CKTstate0 + here->BSIMvbd),
                            CONSTvt0,vcrit,&Check); /* BSIM test*/
                    vbs = vbd + vds;
                }
            } 

             /* determine DC current and derivatives */
            vbd = vbs - vds;
            vgd = vgs - vds;
            vgb = vgs - vbs;


            if(vbs <= 0.0 ) {
                gbs = SourceSatCurrent / CONSTvt0 + ckt->CKTgmin;
                cbs = gbs * vbs ;
            } else {
                evbs = exp(vbs/CONSTvt0);
                gbs = SourceSatCurrent*evbs/CONSTvt0 + ckt->CKTgmin;
                cbs = SourceSatCurrent * (evbs-1) + ckt->CKTgmin * vbs ;
            }
            if(vbd <= 0.0) {
                gbd = DrainSatCurrent / CONSTvt0 + ckt->CKTgmin;
                cbd = gbd * vbd ;
            } else {
                evbd = exp(vbd/CONSTvt0);
                gbd = DrainSatCurrent*evbd/CONSTvt0 +ckt->CKTgmin;
                cbd = DrainSatCurrent *(evbd-1)+ckt->CKTgmin*vbd;
            }
            /* line 400 */
            if(vds >= 0) {
                /* normal mode */
                here->BSIMmode = 1;
            } else {
                /* inverse mode */
                here->BSIMmode = -1;
            }
            /* call BSIMevaluate to calculate drain current and its 
             * derivatives and charge and capacitances related to gate
             * drain, and bulk
             */
           if( vds >= 0 )  {
                BSIMevaluate(vds,vbs,vgs,here,model,&gm,&gds,&gmbs,&qgate,
                    &qbulk,&qdrn,&cggb,&cgdb,&cgsb,&cbgb,&cbdb,&cbsb,&cdgb,
                    &cddb,&cdsb,&cdrain,&von,&vdsat,ckt);
            } else {
                BSIMevaluate(-vds,vbd,vgd,here,model,&gm,&gds,&gmbs,&qgate,
                    &qbulk,&qsrc,&cggb,&cgsb,&cgdb,&cbgb,&cbsb,&cbdb,&csgb,
                    &cssb,&csdb,&cdrain,&von,&vdsat,ckt);
            }
          
            here->BSIMvon = model->BSIMtype * von;
            here->BSIMvdsat = model->BSIMtype * vdsat;  

        

            /*
             *  COMPUTE EQUIVALENT DRAIN CURRENT SOURCE
             */
            cd=here->BSIMmode * cdrain - cbd;
            if ((ckt->CKTmode & (MODETRAN | MODEAC | MODEINITSMSIG)) ||
                    ((ckt->CKTmode & MODETRANOP ) && 
                    (ckt->CKTmode & MODEUIC))) {
                /*
                 *  charge storage elements
                 *
                 *   bulk-drain and bulk-source depletion capacitances
                 *  czbd : zero bias drain junction capacitance
                 *  czbs : zero bias source junction capacitance
                 * czbdsw:zero bias drain junction sidewall capacitance
                 * czbssw:zero bias source junction sidewall capacitance
                 */

                czbd  = model->BSIMunitAreaJctCap * DrainArea;
                czbs  = model->BSIMunitAreaJctCap * SourceArea;
                czbdsw= model->BSIMunitLengthSidewallJctCap * DrainPerimeter;
                czbssw= model->BSIMunitLengthSidewallJctCap * SourcePerimeter;
                PhiB = model->BSIMbulkJctPotential;
                PhiBSW = model->BSIMsidewallJctPotential;
                MJ = model->BSIMbulkJctBotGradingCoeff;
                MJSW = model->BSIMbulkJctSideGradingCoeff;

                /* Source Bulk Junction */
                if( vbs < 0 ) {  
                    arg = 1 - vbs / PhiB;
                    argsw = 1 - vbs / PhiBSW;
                    sarg = exp(-MJ*log(arg));
                    sargsw = exp(-MJSW*log(argsw));
                    *(ckt->CKTstate0 + here->BSIMqbs) =
                        PhiB * czbs * (1-arg*sarg)/(1-MJ) + PhiBSW * 
                    czbssw * (1-argsw*sargsw)/(1-MJSW);
                    capbs = czbs * sarg + czbssw * sargsw ;
                } else {  
                    *(ckt->CKTstate0+here->BSIMqbs) =
                        vbs*(czbs+czbssw)+ vbs*vbs*(czbs*MJ*0.5/PhiB 
                        + czbssw * MJSW * 0.5/PhiBSW);
                    capbs = czbs + czbssw + vbs *(czbs*MJ/PhiB+
                        czbssw * MJSW / PhiBSW );
                }

                /* Drain Bulk Junction */
                if( vbd < 0 ) {  
                    arg = 1 - vbd / PhiB;
                    argsw = 1 - vbd / PhiBSW;
                    sarg = exp(-MJ*log(arg));
                    sargsw = exp(-MJSW*log(argsw));
                    *(ckt->CKTstate0 + here->BSIMqbd) =
                        PhiB * czbd * (1-arg*sarg)/(1-MJ) + PhiBSW * 
                    czbdsw * (1-argsw*sargsw)/(1-MJSW);
                    capbd = czbd * sarg + czbdsw * sargsw ;
                } else {  
                    *(ckt->CKTstate0+here->BSIMqbd) =
                        vbd*(czbd+czbdsw)+ vbd*vbd*(czbd*MJ*0.5/PhiB 
                        + czbdsw * MJSW * 0.5/PhiBSW);
                    capbd = czbd + czbdsw + vbd *(czbd*MJ/PhiB+
                        czbdsw * MJSW / PhiBSW );
                }

            }




            /*
             *  check convergence
             */
            if ( (here->BSIMoff == 0)  || (!(ckt->CKTmode & MODEINITFIX)) ){
                if (Check == 1) {
                    ckt->CKTnoncon++;
#ifndef NEWCONV
                } else {
                    tol=ckt->CKTreltol*MAX(FABS(cdhat),FABS(cd))+ckt->CKTabstol;
                    if (FABS(cdhat-cd) >= tol) { 
                        ckt->CKTnoncon++;
                    } else {
                        tol=ckt->CKTreltol*MAX(FABS(cbhat),FABS(cbs+cbd))+
                            ckt->CKTabstol;
                        if (FABS(cbhat-(cbs+cbd)) > tol) {
                            ckt->CKTnoncon++;
                        }
                    }
#endif /* NEWCONV */
                }
            }
            *(ckt->CKTstate0 + here->BSIMvbs) = vbs;
            *(ckt->CKTstate0 + here->BSIMvbd) = vbd;
            *(ckt->CKTstate0 + here->BSIMvgs) = vgs;
            *(ckt->CKTstate0 + here->BSIMvds) = vds;
            *(ckt->CKTstate0 + here->BSIMcd) = cd;
            *(ckt->CKTstate0 + here->BSIMcbs) = cbs;
            *(ckt->CKTstate0 + here->BSIMcbd) = cbd;
            *(ckt->CKTstate0 + here->BSIMgm) = gm;
            *(ckt->CKTstate0 + here->BSIMgds) = gds;
            *(ckt->CKTstate0 + here->BSIMgmbs) = gmbs;
            *(ckt->CKTstate0 + here->BSIMgbd) = gbd;
            *(ckt->CKTstate0 + here->BSIMgbs) = gbs;

            *(ckt->CKTstate0 + here->BSIMcggb) = cggb;
            *(ckt->CKTstate0 + here->BSIMcgdb) = cgdb;
            *(ckt->CKTstate0 + here->BSIMcgsb) = cgsb;

            *(ckt->CKTstate0 + here->BSIMcbgb) = cbgb;
            *(ckt->CKTstate0 + here->BSIMcbdb) = cbdb;
            *(ckt->CKTstate0 + here->BSIMcbsb) = cbsb;

            *(ckt->CKTstate0 + here->BSIMcdgb) = cdgb;
            *(ckt->CKTstate0 + here->BSIMcddb) = cddb;
            *(ckt->CKTstate0 + here->BSIMcdsb) = cdsb;

            *(ckt->CKTstate0 + here->BSIMcapbs) = capbs;
            *(ckt->CKTstate0 + here->BSIMcapbd) = capbd;

           /* bulk and channel charge plus overlaps */

            if((!(ckt->CKTmode & (MODETRAN | MODEAC))) &&
                    ((!(ckt->CKTmode & MODETRANOP)) ||
                    (!(ckt->CKTmode & MODEUIC)))  && (!(ckt->CKTmode 
                    &  MODEINITSMSIG))) goto line850; 
         
line755:
            if( here->BSIMmode > 0 ) {
                BSIMmosCap(ckt,vgd,vgs,vgb,GateDrainOverlapCap,
                        GateSourceOverlapCap,GateBulkOverlapCap,capbd,capbs
                        ,cggb,cgdb,cgsb,cbgb,cbdb,cbsb,cdgb,cddb,cdsb
                        ,&gcggb,&gcgdb,&gcgsb,&gcbgb,&gcbdb,&gcbsb,&gcdgb
                        ,&gcddb,&gcdsb,&gcsgb,&gcsdb,&gcssb,&qgate,&qbulk
                        ,&qdrn,&qsrc);
            } else {
                BSIMmosCap(ckt,vgs,vgd,vgb,GateSourceOverlapCap,
                    GateDrainOverlapCap,GateBulkOverlapCap,capbs,capbd
                    ,cggb,cgsb,cgdb,cbgb,cbsb,cbdb,csgb,cssb,csdb
                    ,&gcggb,&gcgsb,&gcgdb,&gcbgb,&gcbsb,&gcbdb,&gcsgb
                    ,&gcssb,&gcsdb,&gcdgb,&gcdsb,&gcddb,&qgate,&qbulk
                    ,&qsrc,&qdrn);
            }
             
            if(ByPass) goto line860;
            *(ckt->CKTstate0 + here->BSIMqg) = qgate;
            *(ckt->CKTstate0 + here->BSIMqd) = qdrn -  
                    *(ckt->CKTstate0 + here->BSIMqbd);
            *(ckt->CKTstate0 + here->BSIMqb) = qbulk +  
                    *(ckt->CKTstate0 + here->BSIMqbd) +  
                    *(ckt->CKTstate0 + here->BSIMqbs); 

            /* store small signal parameters */
            if((!(ckt->CKTmode & (MODEAC | MODETRAN))) &&
                    (ckt->CKTmode & MODETRANOP ) && (ckt->CKTmode &
                    MODEUIC ))   goto line850;
            if(ckt->CKTmode & MODEINITSMSIG ) {  
                *(ckt->CKTstate0+here->BSIMcggb) = cggb;
                *(ckt->CKTstate0+here->BSIMcgdb) = cgdb;
                *(ckt->CKTstate0+here->BSIMcgsb) = cgsb;
                *(ckt->CKTstate0+here->BSIMcbgb) = cbgb;
                *(ckt->CKTstate0+here->BSIMcbdb) = cbdb;
                *(ckt->CKTstate0+here->BSIMcbsb) = cbsb;
                *(ckt->CKTstate0+here->BSIMcdgb) = cdgb;
                *(ckt->CKTstate0+here->BSIMcddb) = cddb;
                *(ckt->CKTstate0+here->BSIMcdsb) = cdsb;     
                *(ckt->CKTstate0+here->BSIMcapbd) = capbd;
                *(ckt->CKTstate0+here->BSIMcapbs) = capbs;

                goto line1000;
            }
       
            if(ckt->CKTmode & MODEINITTRAN ) { 
                *(ckt->CKTstate1+here->BSIMqb) =
                    *(ckt->CKTstate0+here->BSIMqb) ;
                *(ckt->CKTstate1+here->BSIMqg) =
                    *(ckt->CKTstate0+here->BSIMqg) ;
                *(ckt->CKTstate1+here->BSIMqd) =
                    *(ckt->CKTstate0+here->BSIMqd) ;
            }
       
       
            error = NIintegrate(ckt,&geq,&ceq,0.0,here->BSIMqb);
            if(error) return(error);
            error = NIintegrate(ckt,&geq,&ceq,0.0,here->BSIMqg);
            if(error) return(error);
            error = NIintegrate(ckt,&geq,&ceq,0.0,here->BSIMqd);
            if(error) return(error);
      
            goto line860;

line850:
            /* initialize to zero charge conductance and current */
            ceqqg = ceqqb = ceqqd = 0.0;
            gcdgb = gcddb = gcdsb = 0.0;
            gcsgb = gcsdb = gcssb = 0.0;
            gcggb = gcgdb = gcgsb = 0.0;
            gcbgb = gcbdb = gcbsb = 0.0;
            goto line900;
            
line860:
            /* evaluate equivalent charge current */
            cqgate = *(ckt->CKTstate0 + here->BSIMiqg);
            cqbulk = *(ckt->CKTstate0 + here->BSIMiqb);
            cqdrn = *(ckt->CKTstate0 + here->BSIMiqd);
            ceqqg = cqgate - gcggb * vgb + gcgdb * vbd + gcgsb * vbs;
            ceqqb = cqbulk - gcbgb * vgb + gcbdb * vbd + gcbsb * vbs;
            ceqqd = cqdrn - gcdgb * vgb + gcddb * vbd + gcdsb * vbs;

            if(ckt->CKTmode & MODEINITTRAN ) {  
                *(ckt->CKTstate1 + here->BSIMiqb) =  
                    *(ckt->CKTstate0 + here->BSIMiqb);
                *(ckt->CKTstate1 + here->BSIMiqg) =  
                    *(ckt->CKTstate0 + here->BSIMiqg);
                *(ckt->CKTstate1 + here->BSIMiqd) =  
                    *(ckt->CKTstate0 + here->BSIMiqd);
            }

            /*
             *  load current vector
             */
line900:
   
            ceqbs = model->BSIMtype * (cbs-(gbs-ckt->CKTgmin)*vbs);
            ceqbd = model->BSIMtype * (cbd-(gbd-ckt->CKTgmin)*vbd);
     
            ceqqg = model->BSIMtype * ceqqg;
            ceqqb = model->BSIMtype * ceqqb;
            ceqqd =  model->BSIMtype * ceqqd;
            if (here->BSIMmode >= 0) {
                xnrm=1;
                xrev=0;
                cdreq=model->BSIMtype*(cdrain-gds*vds-gm*vgs-gmbs*vbs);
            } else {
                xnrm=0;
                xrev=1;
                cdreq = -(model->BSIMtype)*(cdrain+gds*vds-gm*vgd-gmbs*vbd);
            }

            *(ckt->CKTrhs + here->BSIMgNode) -= ceqqg;
            *(ckt->CKTrhs + here->BSIMbNode) -=(ceqbs+ceqbd+ceqqb);
            *(ckt->CKTrhs + here->BSIMdNodePrime) +=
                    (ceqbd-cdreq-ceqqd);
            *(ckt->CKTrhs + here->BSIMsNodePrime) += 
                    (cdreq+ceqbs+ceqqg+ceqqb+ceqqd);

            /*
             *  load y matrix
             */

            *(here->BSIMDdPtr) += (here->BSIMdrainConductance);
            *(here->BSIMGgPtr) += (gcggb);
            *(here->BSIMSsPtr) += (here->BSIMsourceConductance);
            *(here->BSIMBbPtr) += (gbd+gbs-gcbgb-gcbdb-gcbsb);
            *(here->BSIMDPdpPtr) += 
                (here->BSIMdrainConductance+gds+gbd+xrev*(gm+gmbs)+gcddb);
            *(here->BSIMSPspPtr) += 
                (here->BSIMsourceConductance+gds+gbs+xnrm*(gm+gmbs)+gcssb);
            *(here->BSIMDdpPtr) += (-here->BSIMdrainConductance);
            *(here->BSIMGbPtr) += (-gcggb-gcgdb-gcgsb);
            *(here->BSIMGdpPtr) += (gcgdb);
            *(here->BSIMGspPtr) += (gcgsb);
            *(here->BSIMSspPtr) += (-here->BSIMsourceConductance);
            *(here->BSIMBgPtr) += (gcbgb);
            *(here->BSIMBdpPtr) += (-gbd+gcbdb);
            *(here->BSIMBspPtr) += (-gbs+gcbsb);
            *(here->BSIMDPdPtr) += (-here->BSIMdrainConductance);
            *(here->BSIMDPgPtr) += ((xnrm-xrev)*gm+gcdgb);
            *(here->BSIMDPbPtr) += (-gbd+(xnrm-xrev)*gmbs-gcdgb-gcddb-gcdsb);
            *(here->BSIMDPspPtr) += (-gds-xnrm*(gm+gmbs)+gcdsb);
            *(here->BSIMSPgPtr) += (-(xnrm-xrev)*gm+gcsgb);
            *(here->BSIMSPsPtr) += (-here->BSIMsourceConductance);
            *(here->BSIMSPbPtr) += (-gbs-(xnrm-xrev)*gmbs-gcsgb-gcsdb-gcssb);
            *(here->BSIMSPdpPtr) += (-gds-xrev*(gm+gmbs)+gcsdb);


line1000:  ;

        }   /* End of Mosfet Instance */

    }       /* End of Model Instance */
    return(OK);
}

