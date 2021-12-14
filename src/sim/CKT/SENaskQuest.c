/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "SENdefs.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("SENaskQuest.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:47 $")

/* ARGSUSED */
int 
SENaskQuest(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    default:
        return(E_BADPARM);
    }
    /* NOTREACHED */
    return(OK);
}

