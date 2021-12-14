/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTfndTask
     *  find the specified task - not yet supported in spice 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTfndTask.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:35 $")

/* ARGSUSED */
int
CKTfndTask(ckt,taskPtr,taskName)
    GENERIC *ckt;
    GENERIC **taskPtr;
    IFuid  taskName;
{
    return(E_UNSUPP);
}
