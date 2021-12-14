/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTmodParam
     *  attach the given parameter to the specified model in the given circuit
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "util.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTmodParam.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:51 $")

extern SPICEdev *DEVices[];

/* ARGSUSED */
int
CKTmodParam(ckt,modfast,param,val,selector)
    GENERIC *ckt;
    GENERIC *modfast;
    int param;
    IFvalue *val;
    IFvalue *selector;
{
    register int type = ((GENmodel *)modfast)->GENmodType;

    if (((*DEVices[type]).DEVmodParam)) {
        return(((*((*DEVices[type]).DEVmodParam)) (param,val,
                (GENmodel *)modfast)));
    } else {
        return(E_BADPARM);
    }
}
