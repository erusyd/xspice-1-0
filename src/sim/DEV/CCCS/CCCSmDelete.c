/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CCCSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCCSmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:48 $")

int
CCCSmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;

{

    CCCSmodel **model = (CCCSmodel**)inModel;
    CCCSmodel *modfast = (CCCSmodel*)kill;
    CCCSinstance *here;
    CCCSinstance *prev = NULL;
    CCCSmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->CCCSnextModel)) {
        if( (*model)->CCCSmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->CCCSnextModel; /* cut deleted device out of list */
    for(here = (*model)->CCCSinstances ; here ; here = here->CCCSnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
