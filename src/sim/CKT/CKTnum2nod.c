/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTnum2nod
     *  find the given node given its name and return the node pointer
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "SPerror.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("CKTnum2nod.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:01 $")

/* ARGSUSED */
CKTnode *
CKTnum2nod(ckt,node)
    CKTcircuit *ckt;
    int node;
{
    register CKTnode *here;

    for (here = ((CKTcircuit *)ckt)->CKTnodes; here; here = here->next)  {
        if(here->number == node) {
            return(here);
        }
    }
    return((CKTnode *)NULL);
}
