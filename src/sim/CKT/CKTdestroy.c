/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTdestroy(ckt)
     * this is a driver program to iterate through all the various
     * destroy functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "util.h"
#include "IFsim.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTdestroy.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:19 $")

extern SPICEdev *DEVices[];
int
CKTdestroy(inCkt)
    GENERIC *inCkt;
{
    register CKTcircuit *ckt = (CKTcircuit *)inCkt;
    register int i;
    register CKTnode *node;
    register CKTnode *nnode;


    if(ckt->CKTsenInfo){
         if(ckt->CKTrhsOp) FREE(ckt->CKTrhsOp);
         if(ckt->CKTsenRhs) FREE(ckt->CKTsenRhs);
         if(ckt->CKTseniRhs) FREE(ckt->CKTseniRhs);
         SENdestroy(ckt->CKTsenInfo);
    }

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVdestroy != NULL) && (ckt->CKThead[i] != NULL) ){
            (*((*DEVices[i]).DEVdestroy))(&(ckt->CKThead[i]));
        }
    }
    for(i=0;i<=ckt->CKTmaxOrder+1;i++){
        FREE(ckt->CKTstates[i]);
    }
    if(ckt->CKTmatrix)      SMPdestroy(ckt->CKTmatrix);
    if(ckt->CKTbreaks)      FREE(ckt->CKTbreaks);
    for(node = ckt->CKTnodes; node; ) {
        nnode = node->next;
        FREE(node);
        node = nnode;
    }
    ckt->CKTnodes = (CKTnode *)NULL;
    ckt->CKTlastNode = (CKTnode *)NULL;
    FREE(ckt);
    return(OK);
}
