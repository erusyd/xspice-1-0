/*
 * Copyright (c) 1988 Thomas L. Quarles
 */

    /*
     *CKTnames(ckt)
     *  output information on all circuit nodes/equations
     *
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "util.h"
#include "suffix.h"

RCSID("CKTnames.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:53 $")

int
CKTnames(ckt,numNames,nameList)
    register CKTcircuit *ckt;
    int *numNames;
    register IFuid **nameList;
{
    register CKTnode *here;
    register int i;
    *numNames = ckt->CKTmaxEqNum-1;
    *nameList = (IFuid *)MALLOC(*numNames * sizeof(IFuid ));
    if ((*nameList) == (IFuid *)NULL) return(E_NOMEM);
    i=0;
    for (here = ckt->CKTnodes->next; here; here = here->next)  {
        *((*nameList)+i++) = here->name;
    }
    return(OK);
}
