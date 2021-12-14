/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     *CKTnewEq(ckt,node,name)
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

RCSID("CKTnewEq.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:57 $")

int
CKTnewEq(inCkt,node,name)
    GENERIC *inCkt;
    GENERIC **node;
    IFuid name;

{
    CKTnode *mynode;
    register CKTcircuit *ckt = (CKTcircuit *)inCkt;
    int error;

    error = CKTmkNode(ckt,&mynode);
    if(error) return(error);

    if(node) *node = (GENERIC *)mynode;
    mynode->name = name;

    error = CKTlinkEq(ckt,mynode);

    return(error);
}
