/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     *CKTsetNodPm
     *
     *   set a parameter on a node.
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

RCSID("CKTsetNodPm.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:22 $")

/* ARGSUSED */
int
CKTsetNodPm(ckt,node,parm,value,selector)
    GENERIC *ckt;
    GENERIC *node;
    int parm;
    IFvalue *value;
    IFvalue *selector;

{
    if(!node) return(E_BADPARM);
    switch(parm) {

    case PARM_NS:
        ((CKTnode *)node)->nodeset = value->rValue;
        ((CKTnode *)node)->nsGiven = 1;
        break;

    case PARM_IC:
        ((CKTnode *)node)->ic = value->rValue;
        ((CKTnode *)node)->icGiven = 1;
        break;

    case PARM_NODETYPE:
        ((CKTnode *)node)->type = value->iValue;
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}
