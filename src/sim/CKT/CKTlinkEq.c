/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     *CKTlinkEq
     *  Link an already allocated node into the necessary structure
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

RCSID("CKTlinkEq.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:40 $")

int
CKTlinkEq(ckt,node)
    CKTcircuit *ckt;
    CKTnode *node;
{
    if(!(ckt->CKTnodes)) { /*  starting the list - allocate both ground and 1 */
        ckt->CKTnodes = (CKTnode *) MALLOC(sizeof(CKTnode));
        if(ckt->CKTnodes == (CKTnode *)NULL) return(E_NOMEM);
        ckt->CKTnodes->name = (char *)NULL;
        ckt->CKTnodes->type = SP_VOLTAGE;
        ckt->CKTnodes->number = 0;
        ckt->CKTlastNode = ckt->CKTnodes;
    }
    if(node == (CKTnode *)NULL) return(E_BADPARM);
    ckt->CKTlastNode->next = node;
    ckt->CKTlastNode = ckt->CKTlastNode->next;
    ckt->CKTlastNode->number = ckt->CKTmaxEqNum++;
    ckt->CKTlastNode->next = (CKTnode *)NULL;
    return(OK);
}
