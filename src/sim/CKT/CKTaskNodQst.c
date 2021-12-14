/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     *CKTaskNodQst
     *
     *   ask about a parameter on a node.
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CKTdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTaskNodQst.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:11 $")

/* ARGSUSED */
int
CKTaskNodQst(ckt,node,parm,value,selector)
    GENERIC *ckt;
    GENERIC *node;
    int parm;
    IFvalue *value;
    IFvalue *selector;

{
    if(!node) return(E_BADPARM);
    switch(parm) {

    case PARM_NS:
        value->rValue = ((CKTnode *)node)->nodeset;
        break;

    case PARM_IC:
        value->rValue = ((CKTnode *)node)->ic;
        break;

    case PARM_NODETYPE:
        value->iValue = ((CKTnode *)node)->type;
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}
