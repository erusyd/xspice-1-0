/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "TRAdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("TRAmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:39 $")

int
TRAmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    TRAmodel **model = (TRAmodel **)inModel;
    TRAmodel *modfast = (TRAmodel *)kill;
    TRAinstance *here;
    TRAinstance *prev = NULL;
    TRAmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->TRAnextModel)) {
        if( (*model)->TRAmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->TRAnextModel; /* cut deleted device out of list */
    for(here = (*model)->TRAinstances ; here ; here = here->TRAnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
