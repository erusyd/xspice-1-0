/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:40 $")

int
DIOmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    DIOmodel **model = (DIOmodel**)inModel;
    DIOmodel *modfast = (DIOmodel*)kill;
    DIOinstance *here;
    DIOinstance *prev = NULL;
    DIOmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->DIOnextModel)) {
        if( (*model)->DIOmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->DIOnextModel; /* cut deleted device out of list */
    for(here = (*model)->DIOinstances ; here ; here = here->DIOnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
