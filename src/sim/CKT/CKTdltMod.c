/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTdltMod
     *  delete the specified model - not yet supported in spice 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTdltMod.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:22 $")

/* ARGSUSED */
int
CKTdltMod(ckt,modPtr)
    GENERIC *ckt;
    GENERIC *modPtr;
{
    return(E_UNSUPP);
}
