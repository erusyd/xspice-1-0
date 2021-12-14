/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     *CKTnodName(ckt)
     *  output information on all circuit nodes/equations
     *
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "suffix.h"

RCSID("CKTnodName.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:00 $")

IFuid
CKTnodName(ckt,nodenum)
    CKTcircuit *ckt;
    register int nodenum;
{
    register CKTnode *here;

    for(here = ckt->CKTnodes;here; here = here->next) {
        if(here->number == nodenum) { 
            /* found it */
            return(here->name);
        }
    }
    /* doesn't exist - do something */
    return("UNKNOWN NODE");
}
