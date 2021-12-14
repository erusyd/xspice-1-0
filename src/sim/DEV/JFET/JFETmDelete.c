/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "JFETdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("JFETmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:47 $")

int
JFETmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    JFETmodel **model = (JFETmodel**)inModel;
    JFETmodel *modfast = (JFETmodel*)kill;
    JFETinstance *here;
    JFETinstance *prev = NULL;
    JFETmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->JFETnextModel)) {
        if( (*model)->JFETmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->JFETnextModel; /* cut deleted device out of list */
    for(here = (*model)->JFETinstances ; here ; here = here->JFETnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
