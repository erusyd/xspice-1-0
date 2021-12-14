/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     *CKTnewNode(ckt,node,name)
     *  Allocate a new circuit equation number (returned) in the specified
     *  circuit to contain a new equation or node
     * returns -1 for failure to allocate a node number 
     *
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTnewNode.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:58 $")

/* should just call CKTnewEQ and set node type afterwards */
int
CKTnewNode(inCkt,node,name)
    GENERIC *inCkt;
    GENERIC **node;
    IFuid name;

{
    register CKTcircuit *ckt = (CKTcircuit *)inCkt;
    if(!(ckt->CKTnodes)) { /*  starting the list - allocate both ground and 1 */
        ckt->CKTnodes = (CKTnode *) MALLOC(sizeof(CKTnode));
        if(ckt->CKTnodes == (CKTnode *)NULL) return(E_NOMEM);
        ckt->CKTnodes->name = (char *)NULL;
        ckt->CKTnodes->type = SP_VOLTAGE;
        ckt->CKTnodes->number = 0;
        ckt->CKTlastNode = ckt->CKTnodes;
    }
    ckt->CKTlastNode->next = (CKTnode *)MALLOC(sizeof(CKTnode));
    if(ckt->CKTlastNode->next == (CKTnode *)NULL) return(E_NOMEM);
    ckt->CKTlastNode = ckt->CKTlastNode->next;
    ckt->CKTlastNode->name = name;
    ckt->CKTlastNode->number = ckt->CKTmaxEqNum++;
    ckt->CKTlastNode->type = SP_VOLTAGE;
    ckt->CKTlastNode->next = (CKTnode *)NULL;

    if(node) *node = (GENERIC *)ckt->CKTlastNode;
    return(OK);
}
