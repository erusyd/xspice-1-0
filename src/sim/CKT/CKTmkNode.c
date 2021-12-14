/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     *CKTmkNode(ckt,node)
     *  Tentatively allocate a new circuit equation structure
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

RCSID("CKTmkNode.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:46 $")

/* ARGSUSED */
int
CKTmkNode(ckt,node)
    CKTcircuit *ckt;
    CKTnode **node;

{
    CKTnode *mynode;

    mynode = (CKTnode *)MALLOC(sizeof(CKTnode));
    if(mynode == (CKTnode *)NULL) return(E_NOMEM);
    mynode->next = (CKTnode *)NULL;

    if(node) *node = mynode;
    return(OK);
}
