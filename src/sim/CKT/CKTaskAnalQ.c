/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "TSKdefs.h"
#include "JOBdefs.h"
#include "CKTdefs.h"
#include "IFsim.h"
#include "util.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "suffix.h"

RCSID("CKTaskAnalQ.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:10 $")

extern SPICEanalysis *analInfo[];

/* ARGSUSED */
int
CKTaskAnalQ(ckt,analPtr,parm,value,selector)
    GENERIC *ckt;
    GENERIC *analPtr;
    int parm;
    IFvalue *value;
    IFvalue *selector;
{
    register int type = ((JOB *)analPtr)->JOBtype;

    if((analInfo[type]->askQuest) == NULL) return(E_BADPARM);
    return( (*(analInfo[type]->askQuest))((CKTcircuit*)ckt,analPtr,parm,value));
}
