/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTground(ckt,node)
     *  specify the node to be the ground node of the given circuit
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTground.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:36 $")

int
CKTground(inCkt,node,name)
    GENERIC *inCkt;
    GENERIC **node;
    IFuid name;

{
    register CKTcircuit *ckt = (CKTcircuit *)inCkt;

    if(ckt->CKTnodes) {
        if(ckt->CKTnodes->name) {
            /*already exists - keep old name, but return it */
            if(node)*node = (char *)ckt->CKTnodes;
            return(E_EXISTS);
        }
        ckt->CKTnodes->name = name;
        ckt->CKTnodes->type = SP_VOLTAGE;
        ckt->CKTnodes->number = 0;
    } else {
        ckt->CKTnodes = (CKTnode *)MALLOC(sizeof(CKTnode));
        if(ckt->CKTnodes == NULL) return(E_NOMEM);
        ckt->CKTnodes->name = name;
        ckt->CKTnodes->type = SP_VOLTAGE;
        ckt->CKTnodes->number = 0;
        ckt->CKTnodes->next = (CKTnode *)NULL;
        ckt->CKTlastNode = ckt->CKTnodes;
    }
    if(node)*node = (char *)ckt->CKTnodes;
    return(OK);

}
