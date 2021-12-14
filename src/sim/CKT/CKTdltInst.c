/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTdltInst
     *  delete the specified instance - not yet supported in spice 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTdltInst.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:21 $")

/* ARGSUSED */
int
CKTdltInst(ckt,instance)
    GENERIC *ckt;
    GENERIC *instance;
{
    return(E_UNSUPP);
}
