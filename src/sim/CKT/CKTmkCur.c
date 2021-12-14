/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTmkCur
     *  make the given name a 'node' of type current in the 
     * specified circuit
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "SPerror.h"
#include "CKTdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTmkCur.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:44 $")

/* ARGSUSED */
int
CKTmkCur(ckt,node,basename,suffix)
    CKTcircuit  *ckt;
    CKTnode **node;
    IFuid basename;
    char *suffix;
{
    IFuid uid;
    int error;
    CKTnode *mynode;
    CKTnode *checknode;

    error = CKTmkNode(ckt,&mynode);
    if(error) return(error);
    checknode = mynode;
    error = (*(SPfrontEnd->IFnewUid))((GENERIC *)ckt,&uid,basename,
            suffix,UID_SIGNAL,(GENERIC**)&checknode);
    if(error) {
        FREE(mynode);
        if(node) *node = checknode;
        return(error);
    }
    mynode->name = uid;
    mynode->type = SP_CURRENT;
    if(node) *node = mynode;
    error = CKTlinkEq(ckt,mynode);
    return(error);
}
