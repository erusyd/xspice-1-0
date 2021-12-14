/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTparam
     *  attach the given parameter to the specified device in the given circuit
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "util.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTparam.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:06 $")

extern SPICEdev *DEVices[];

/* ARGSUSED */
int
CKTparam(ckt,fast,param,val,selector)
    GENERIC *ckt;
    GENERIC *fast;
    int param;
    IFvalue *val;
    IFvalue *selector;
{
    register int type;
    GENinstance *myfast = (GENinstance *)fast;
    type = myfast->GENmodPtr->GENmodType;
    if(((*DEVices[type]).DEVparam)) {
        return(((*((*DEVices[type]).DEVparam)) (param,val,myfast,selector)));
    } else {
        return(E_BADPARM);
    }
}
