/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "BSIMdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BSIMmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:08 $")

int
BSIMmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;

{
    BSIMmodel **model = (BSIMmodel**)inModel;
    BSIMmodel *modfast = (BSIMmodel*)kill;
    BSIMinstance *here;
    BSIMinstance *prev = NULL;
    BSIMmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->BSIMnextModel)) {
        if( (*model)->BSIMmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->BSIMnextModel; /* cut deleted device out of list */
    for(here = (*model)->BSIMinstances ; here ; here = here->BSIMnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
