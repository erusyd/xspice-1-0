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
#include "TRCVdefs.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("TFaskQuest.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:58 $")

/* ARGSUSED */
int 
TFaskQuest(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    default:
        return(E_BADPARM);
    }
    /* NOTREACHED */ /* TEMPORARY until cases get added */
    return(OK);
}

