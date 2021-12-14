/*
 * Copyright (c) 1985 Mani B. Srivastava
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "complex.h"
#include "CKTdefs.h"
#include "SMPdefs.h"
#include "PZdefs.h"
#include "TRANdefs.h"   /* only to get the 'mode' definitions */
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("PZan.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:44 $")

/* ARGSUSED */
int
PZan(ckt,reset) 
CKTcircuit *ckt;
int reset;
{

    register PZAN *pzptr = (PZAN *)ckt->CKTcurJob;
    int error;
    char *name;
    int i,type;
    int j;
    int typeCkt;
    int NIpzMuller();
    int NIpzSolve();
    int numpole,numzero;
    IFuid *namelist;    /* names of output variables */
    GENERIC *pzPlotPtr; /* the plot pointer for front end */
    IFcomplex *out_list;    /* out list of complex data points */
    IFvalue outData;    /* output variable (points to out_list) */
    IFvalue refVal; /* reference variable (always 0)*/
    root *temproot;
    int again;
static char *short1msg = "Input port is shorted";
static char *short2msg = "Output port is shorted";
static char *inoutmsg = "Transfer function is 1, no poles or zeros to find";
static char *xmline = "Transmission lines not supported by pole-zero analysis";


    pzptr->PZnumswaps=1;
    pzptr->PZpoleList = (root *)NULL;
    pzptr->PZzeroList = (root *)NULL;
    if(pzptr->PZnodeI==0) {
        pzptr->PZnodeI=pzptr->PZnodeG;
        pzptr->PZnodeG=0;
    }
    if(pzptr->PZnodeJ==0) {
        pzptr->PZnodeJ=pzptr->PZnodeK;
        pzptr->PZnodeK=0;
    }
    if(pzptr->PZnodeI == pzptr->PZnodeG) {
        errMsg = MALLOC(strlen(short1msg)+1);
        strcpy(errMsg,short1msg);
        return(E_SHORT);
    }
    if(pzptr->PZnodeJ == pzptr->PZnodeK) {
        errMsg = MALLOC(strlen(short2msg)+1);
        strcpy(errMsg,short2msg);
        return(E_SHORT);
    }
    if( (pzptr->PZnodeI==pzptr->PZnodeK) && (pzptr->PZnodeG==pzptr->PZnodeJ) ){
        pzptr->PZnodeI = pzptr->PZnodeJ;
        pzptr->PZnodeG = pzptr->PZnodeK;
    }
    if( (pzptr->PZnodeI==pzptr->PZnodeJ) && (pzptr->PZnodeG==pzptr->PZnodeK) &&
            (pzptr->PZflagVI==0) ) {
        errMsg = MALLOC(strlen(inoutmsg)+1);
        strcpy(errMsg,inoutmsg);
        return(E_INISOUT);
    }
    again=0;
    error = CKTop(ckt,
            (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITJCT,
            (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITFLOAT,
            ckt->CKTdcMaxIter);
    if(error) return(error);
    ckt->CKTmode = (ckt->CKTmode&MODEUIC) | MODEDCOP | MODEINITSMSIG;
    error = CKTload(ckt);
    if(error) return(error);

    ckt->CKTmode=MODEAC ;
/*check if there are any transmission lines*/
    i=CKTtypelook("transmission line");
    if (i!=-1) {
        if (ckt->CKThead[i]!=NULL) {
            errMsg = MALLOC(strlen(xmline)+1);
            strcpy(errMsg,xmline);
            return(E_XMISSIONLINE);
        }
    }
    loop:
    if (again==1) {
        error = CKTop(ckt, 
                (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITJCT, 
                (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITFLOAT,
                ckt->CKTdcMaxIter);
        if(error) return(error);
        ckt->CKTmode = (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITSMSIG;
        error = CKTload(ckt);
        if(error) return(error);
    };
    pzptr->PZJK_Jptr=(double *)NULL;
    pzptr->PZJK_Kptr=(double *)NULL;
    pzptr->PZJK_JKptr=(double *)NULL;
    pzptr->PZIG_Iptr=(double *)NULL;
    pzptr->PZIG_Gptr=(double *)NULL;
    pzptr->PZIG_IGptr=(double *)NULL;
/*rest of the SMP has already been setup by the calling routine
  so now we just form an additional equation for the variable
  V sub JK which is the output port voltage and create pointers
  to SMP elements at (jk,j),(jk,k),(jk,jk),(ig,i),(ig,g),(ig,ig)*/
    if ((pzptr->PZflagVI==0) && (pzptr->PZnodeK ==0) && (pzptr->PZnodeG==0)) {
        /*transfer function of the type V/V*/
        /*matrix remains as it is*/
        typeCkt=1;
    };
    if ((pzptr->PZflagVI==0) && (pzptr->PZnodeK ==0) && (pzptr->PZnodeG!=0)) {
        /*transfer function of the type V/V*/
        typeCkt=2;
        /*introduce row i-g*/
        pzptr->PZdiffIG=ckt->CKTmaxEqNum;
        if ((pzptr->PZIG_Iptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffIG,
                pzptr->PZnodeI))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZIG_Gptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffIG,
                pzptr->PZnodeG))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZIG_IGptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffIG,
                pzptr->PZdiffIG))==(double *)(NULL)) return(E_NOMEM);
    };
    if ((pzptr->PZflagVI==0) && (pzptr->PZnodeK !=0) && (pzptr->PZnodeG==0)) {
        /*transfer function of the type V/V*/
        typeCkt=3;
        /*introduce row j-k*/
        pzptr->PZdiffJK=ckt->CKTmaxEqNum;
        if ((pzptr->PZJK_Jptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeJ))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_Kptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeK))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_JKptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZdiffJK))==(double *)(NULL)) return(E_NOMEM);
    };
    if ((pzptr->PZflagVI==0) && (pzptr->PZnodeK !=0) && (pzptr->PZnodeG!=0) &&
            (pzptr->PZnodeI==pzptr->PZnodeJ)&&(pzptr->PZnodeG==pzptr->PZnodeK)){
        /*transfer function of the type V/V*/
        typeCkt=4;
        /*introduce row j-k = i-g*/
        pzptr->PZdiffJK=ckt->CKTmaxEqNum;
        if ((pzptr->PZJK_Jptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeJ))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_Kptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeK))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_JKptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZdiffJK))==(double *)(NULL)) return(E_NOMEM);
        pzptr->PZdiffIG=pzptr->PZdiffJK;
        pzptr->PZIG_Iptr=pzptr->PZJK_Jptr;
        pzptr->PZIG_Gptr=pzptr->PZJK_Kptr;
        pzptr->PZIG_IGptr=pzptr->PZJK_JKptr;
    };
    if ((pzptr->PZflagVI==0) && (pzptr->PZnodeK !=0) && (pzptr->PZnodeG!=0) &&
            !((pzptr->PZnodeI==pzptr->PZnodeJ) &&
            (pzptr->PZnodeG==pzptr->PZnodeK))) {
        /*transfer function of the type V/V*/
        typeCkt=5;
        /*introduce row j-k*/
        pzptr->PZdiffJK=ckt->CKTmaxEqNum;
        if ((pzptr->PZJK_Jptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeJ))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_Kptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeK))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_JKptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZdiffJK))==(double *)(NULL)) return(E_NOMEM);
        /*introduce row i-g*/
        pzptr->PZdiffIG=ckt->CKTmaxEqNum+1;
        if ((pzptr->PZIG_Iptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffIG,
                pzptr->PZnodeI))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZIG_Gptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffIG,
                pzptr->PZnodeG))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZIG_IGptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffIG,
                pzptr->PZdiffIG))==(double *)(NULL)) return(E_NOMEM);
    };
    if ((pzptr->PZflagVI==1) && (pzptr->PZnodeK !=0)) {
        /*transfer function of the type V/I*/
        typeCkt=6;
        /*introduce row j-k*/
        pzptr->PZdiffJK=ckt->CKTmaxEqNum;
        if ((pzptr->PZJK_Jptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeJ))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_Kptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZnodeK))==(double *)(NULL)) return(E_NOMEM);
        if ((pzptr->PZJK_JKptr=SMPmakeElt(ckt->CKTmatrix, pzptr->PZdiffJK,
                pzptr->PZdiffJK))==(double *)(NULL)) return(E_NOMEM);
    };
    if ((pzptr->PZflagVI==1) && (pzptr->PZnodeK ==0)) {
    /*transfer function of the type V/I*/
        typeCkt=7;
        /*matrix remains as it is*/
    };
/*now we can go ahead and find the
  poles and zeros depending on what is required 
  the type of analysis needed is shown by PZflagPZ whose values
  have the following interpretation:
         1 - pole analysis only
         2 - zero analysis only
         3 - both
*/
    NIdestroy(ckt);
    error=NIinit(ckt);
    if (error) return(error);
    error=CKTpzSetup(ckt);
    if (error) return(error);
    error=CKTtemp(ckt);
    if (error) return(error);

    if ((pzptr->PZflagPZ==1) || (pzptr->PZflagPZ==3)) {
        /*pole analysis*/
        type=typeCkt;
        error=NIpzMuller(ckt,&(pzptr->PZpoleList), type);
        if (error) return(error);
    }
    if (pzptr->PZflagPZ==3) {
        pzptr->PZflagPZ=2;
        pzptr->PZnumswaps=1;
        again=1;
        goto loop;
    }; 
    if ((pzptr->PZflagPZ==2) || (pzptr->PZflagPZ==3)) {
        /*zero analysis*/
        type=typeCkt+10;
        error=NIpzMuller(ckt,&(pzptr->PZzeroList), type);
        if (error) return(error);
    }
    numpole=0;
    temproot=pzptr->PZpoleList;
    while (temproot != (root *)(NULL)) {
        numpole++;
        temproot=temproot->next;
    };
    numzero=0;
    temproot=pzptr->PZzeroList;
    while (temproot != (root *)(NULL)) {
        numzero++;
        temproot=temproot->next;
    };
    namelist = (IFuid *)MALLOC((numpole+numzero)*sizeof(IFuid));
    name = (char *) MALLOC(10 *  sizeof(char));
    if (name == (char *)NULL) return(E_NOMEM);
    j=0;
    for (i=0 ; i<numpole ; i++) {
        (void) sprintf(name,"pole(%-u)",(i+1));
        (*(SPfrontEnd->IFnewUid))(ckt,&(namelist[j++]),(IFuid)NULL,
            name,UID_OTHER,(GENERIC **)NULL);
    };
    for (i=0 ; i<numzero ; i++) {
        (void) sprintf(name,"zero(%-u)",(i+1));
        (*(SPfrontEnd->IFnewUid))(ckt,&(namelist[j++]),(IFuid)NULL,
            name,UID_OTHER,(GENERIC **)NULL);
    };
    (*(SPfrontEnd->OUTpBeginPlot))(ckt,(GENERIC *)pzptr,pzptr->JOBname,
            (IFuid)NULL,(int)0,(int)(numpole+numzero),namelist,IF_COMPLEX,
            &pzPlotPtr);
    out_list=
        (IFcomplex *)MALLOC((numpole+numzero)*sizeof(IFcomplex));
    if (out_list == (IFcomplex *)NULL) return(E_NOMEM);
    temproot=pzptr->PZpoleList;
    for (i=0 ; i<numpole ; i++) {
        if (fabs((temproot->real))<1.0e-20) temproot->real=0.0;
        if (fabs((temproot->imag))<1.0e-20) temproot->imag=0.0;
        out_list[i].real = (temproot->real) * 1.0e6;
        out_list[i].imag = (temproot->imag) * 1.0e6;
        temproot=temproot->next;
    };
    temproot=pzptr->PZzeroList;
    for (i=numpole ; i<(numpole + numzero) ; i++) {
        if (fabs((temproot->real))<1.0e-20) temproot->real=0.0;
        if (fabs((temproot->imag))<1.0e-20) temproot->imag=0.0;
        out_list[i].real = (temproot->real) * 1.0e6;
        out_list[i].imag = (temproot->imag) * 1.0e6;
        temproot=temproot->next;
    };

    outData.v.numValue=numpole+numzero;
    outData.v.vec.cVec=out_list;
    (*(SPfrontEnd->OUTpData))(pzPlotPtr,&refVal,&outData);

    (*(SPfrontEnd->OUTendPlot))(pzPlotPtr);
    return(OK);
}
