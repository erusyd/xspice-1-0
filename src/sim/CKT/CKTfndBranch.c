/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTfndBranch(ckt,name)
     * this is a driver program to iterate through all the various
     * findBranch functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "suffix.h"

RCSID("CKTfndBranch.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:29 $")

int
CKTfndBranch(ckt,name)
    register CKTcircuit *ckt;
    IFuid name;

{
    extern SPICEdev *DEVices[];

    register int i;
    int j;

    for (i=0;i<DEVmaxnum;i++) {
        if ((*DEVices[i]).DEVfindBranch != NULL && ckt->CKThead[i] != NULL) {
            j = (*((*DEVices[i]).DEVfindBranch))(ckt,ckt->CKThead[i],name);
            if(j != 0) return(j);
        }
    }
    return(0);
}
