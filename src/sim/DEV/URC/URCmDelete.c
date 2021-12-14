/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "URCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("URCmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:53 $")

int
URCmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    URCmodel **model = (URCmodel**)inModel;
    URCmodel *modfast = (URCmodel *)kill;
    URCinstance *here;
    URCinstance *prev = NULL;
    URCmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->URCnextModel)) {
        if( (*model)->URCmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->URCnextModel; /* cut deleted device out of list */
    for(here = (*model)->URCinstances ; here ; here = here->URCnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
