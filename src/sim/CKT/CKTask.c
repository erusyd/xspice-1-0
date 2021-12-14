/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTask
     *  Ask questions about a specified device.
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTask.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:09 $")

extern SPICEdev *DEVices[];

int
CKTask(ckt,fast,which,value,selector)
    GENERIC *ckt;
    GENERIC *fast;
    int which;
    IFvalue *value;
    IFvalue *selector;
{
    register int type = ((GENinstance *)fast)->GENmodPtr->GENmodType;
    if((*DEVices[type]).DEVask) {
        return( (*((*DEVices[type]).DEVask)) ((CKTcircuit *)ckt,
                (GENinstance *)fast,which,value,selector) );
    }
    return(E_BADPARM);
}
