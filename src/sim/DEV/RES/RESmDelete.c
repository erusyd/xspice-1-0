/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "RESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("RESmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:56 $")

int
RESmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    RESmodel **model = (RESmodel **)inModel;
    RESmodel *modfast = (RESmodel *)kill;
    RESinstance *here;
    RESinstance *prev = NULL;
    RESmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->RESnextModel)) {
        if( (*model)->RESmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->RESnextModel; /* cut deleted device out of list */
    for(here = (*model)->RESinstances ; here ; here = here->RESnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
