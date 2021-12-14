/*
 * Copyright (c) 1985 Mani B. srivastava
 */

    /* CKTpzLoad(ckt,s,type)
     * this is a driver program to iterate through all the various
     * pz load functions provided for the circuit elements in the
     * given circuit 
     *It also loads the proper values into the new column and row
     *introduced for pole-zero analysis
     *Besides , depending upon the value of type it takes action
     *like  replacing a column with r.h.s
     */

#include "prefix.h"
#include <stdio.h>
#include "PZdefs.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "complex.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTpzLoad.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:07 $")

extern SPICEdev *DEVices[];
int
CKTpzLoad(ckt,s,type)
    register CKTcircuit *ckt;
    SPcomplex *s;
    int type;
{
    register PZAN *pzptr = (PZAN *)ckt->CKTcurJob;
    register int i;
    int error;
    int size;
    int SMPmatSize();
    int rowdel,coldel;
    int SMProwcolDel();
    register SMPelement *item;
    int n;
    SMPelement *SMPfindElt();
    size = SMPmatSize(ckt->CKTmatrix);
    for (i=0;i<=size;i++) {
        *(ckt->CKTrhs+i)=0;
        *(ckt->CKTirhs+i)=0;
    }
    SMPcClear(ckt->CKTmatrix);

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVpzLoad != NULL) && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVpzLoad))(ckt->CKThead[i],ckt,s);
            if(error) return(error);
        }
    }
    if (pzptr->PZJK_Jptr !=(double *)NULL) {
        *(pzptr->PZJK_Jptr)= -1.0;
    }
    if (pzptr->PZJK_Kptr !=(double *)NULL) {
        *(pzptr->PZJK_Kptr)=1.0;
    }
    if (pzptr->PZJK_JKptr !=(double *)NULL) {
        *(pzptr->PZJK_JKptr)=1.0;
    }
    if (pzptr->PZIG_Iptr !=(double *)NULL) {
        *(pzptr->PZIG_Iptr)= -1.0;
    }
    if (pzptr->PZIG_Gptr !=(double *)NULL) {
        *(pzptr->PZIG_Gptr)=1.0;
    }
    if (pzptr->PZIG_IGptr !=(double *)NULL) {
        *(pzptr->PZIG_IGptr)=1.0;
    }
    switch (type) {
    case 1:
    case 3:
          /*substitute r.h.s. in col i*/
          coldel=pzptr->PZnodeI;
          break;
    case 2:
    case 5:
          /*substitute r.h.s. in col i-g*/
          coldel=pzptr->PZdiffIG;
          break;
    case 4:
    case 13:
    case 14:
    case 15:
    case 16:
          /*substitute r.h.s. in col j-k*/
          coldel=pzptr->PZdiffJK;
          break;
    case 6:
    case 7:
          /*matrix remains as it is*/
          return(OK);
    case 11:
    case 12:
    case 17:
          /*substitute r.h.s. in col j*/
          coldel=pzptr->PZnodeJ;
          break;
    };
    coldel = SMPextToIntMapCol(coldel,ckt->CKTmatrix);
        /*walk down the column*/
    for (n=1;n<=ckt->CKTmatrix->SMPsize;n++) {
        item = SMPfindElt(ckt->CKTmatrix,n,coldel,0);
        if(item != (SMPelement *)NULL) {
                /*the element exists so make it 0+j0*/
            item->SMPvalue=0.0;
            item->SMPiValue=0.0;
        }
    }
    rowdel = SMPextToIntMapRow(pzptr->PZnodeI,ckt->CKTmatrix);
    item=SMPfindElt(ckt->CKTmatrix,rowdel,coldel,1);
    if (item == (SMPelement *)NULL) return(E_NOMEM);
    item->SMPvalue=1.0;
    item->SMPiValue=0.0;
    if (pzptr->PZnodeG !=0) {
        rowdel = SMPextToIntMapRow(pzptr->PZnodeG,ckt->CKTmatrix);
        item=SMPfindElt(ckt->CKTmatrix,rowdel,coldel,1);
        if (item == (SMPelement *)NULL) return(E_NOMEM);
        item->SMPvalue= -1.0;
        item->SMPiValue=0.0;
    }
    if ((type==4) || (type==13) || (type==14) || (type==15) || (type==16)) {
        *(pzptr->PZJK_JKptr)= 1.0;
    }
    if ((type==2) || (type==5)) {
        *(pzptr->PZIG_IGptr)= 1.0;
    }
    return(OK);
}
