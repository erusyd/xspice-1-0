/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CCVSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCVSmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:05 $")

int
CCVSmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    CCVSmodel **model = (CCVSmodel**)inModel;
    CCVSmodel *modfast = (CCVSmodel*)kill;
    CCVSinstance *here;
    CCVSinstance *prev = NULL;
    CCVSmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->CCVSnextModel)) {
        if( (*model)->CCVSmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->CCVSnextModel; /* cut deleted device out of list */
    for(here = (*model)->CCVSinstances ; here ; here = here->CCVSnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
