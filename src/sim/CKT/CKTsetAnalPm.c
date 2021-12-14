/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "TSKdefs.h"
#include "JOBdefs.h"
#include "IFsim.h"
#include "util.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CKTdefs.h"
#include "suffix.h"

RCSID("CKTsetAnalPm.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:20 $")

extern SPICEanalysis *analInfo[];

/* ARGSUSED */
int
CKTsetAnalPm(ckt,analPtr,parm,value,selector)
    GENERIC *ckt;
    GENERIC *analPtr;
    int parm;
    IFvalue *value;
    IFvalue *selector;
{
    register int type = ((JOB *)analPtr)->JOBtype;
    if((analInfo[type]->setParm)==NULL) return(E_BADPARM);
    return( (*(analInfo[type]->setParm))(ckt,analPtr,parm,value) );
}
