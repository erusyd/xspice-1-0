/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTdltNod
     *  delete the specified node - not yet supported in spice 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTdltNod.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:23 $")

/* ARGSUSED */
int
CKTdltNod(ckt,node)
    GENERIC *ckt;
    GENERIC *node;
{
    return(E_UNSUPP);
}
