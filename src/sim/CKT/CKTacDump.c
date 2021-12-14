/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTacDump(ckt,freq,file)
     * this is a simple program to dump the complex rhs vector 
     * into the rawfile.
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "IFsim.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTacDump.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:03 $")

int
CKTacDump(ckt,freq,plot)
    register CKTcircuit *ckt;
    double freq;
    GENERIC *plot;
{
    register double *rhsold;
    register double *irhsold;
    register int i;
    register IFcomplex *data;
    IFvalue freqData;
    IFvalue valueData;

    rhsold = ckt->CKTrhsOld;
    irhsold = ckt->CKTirhsOld;
    freqData.rValue = freq;
    valueData.v.numValue = ckt->CKTmaxEqNum-1;
    data = (IFcomplex *) MALLOC((ckt->CKTmaxEqNum-1)*sizeof(IFcomplex));
    valueData.v.vec.cVec = data;
    for (i=0;i<ckt->CKTmaxEqNum-1;i++) {
        data[i].real = rhsold[i+1];
        data[i].imag = irhsold[i+1];
    }
    (*(SPfrontEnd->OUTpData))(plot,&freqData,&valueData);
    FREE(data);
    return(OK);
}
