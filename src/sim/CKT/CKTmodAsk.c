/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTmodAsk
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

RCSID("CKTmodAsk.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:49 $")

extern SPICEdev *DEVices[];

/* ARGSUSED */
int
CKTmodAsk(ckt,modfast,which,value,selector)
    GENERIC *ckt;
    GENERIC *modfast;
    int which;
    IFvalue *value;
    IFvalue *selector;
{
    int type = ((GENmodel *)modfast)->GENmodType;
    if((*DEVices[type]).DEVmodAsk) {
        return( (*((*DEVices[type]).DEVmodAsk)) ((CKTcircuit *)ckt,
                (GENmodel *)modfast,which,value) );
    }
    return(E_BADPARM);
}
