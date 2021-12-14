/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTfndNode
     *  find the given node given its name and return the node pointer
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "SPerror.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("CKTfndNode.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:34 $")

/* ARGSUSED */
int
CKTfndNode(ckt,node,name)
    GENERIC *ckt;
    GENERIC **node;
    IFuid name;
{
    register CKTnode *here;

    for (here = ((CKTcircuit *)ckt)->CKTnodes; here; here = here->next)  {
        if(here->name == name) {
            if(node) *node = (char *)here;
            return(OK);
        }
    }
    return(E_NOTFOUND);
}
