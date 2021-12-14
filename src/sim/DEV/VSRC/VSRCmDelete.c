/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "VSRCdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VSRCmDelete.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:46 $")

int
VSRCmDelete(inModel,modname,fast)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *fast;
{
    VSRCmodel **model = (VSRCmodel **)inModel;
    VSRCmodel *modfast = (VSRCmodel *)fast;
    VSRCinstance *here;
    VSRCinstance *prev = NULL;
    VSRCmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->VSRCnextModel)) {
        if( (*model)->VSRCmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->VSRCnextModel; /* cut deleted device out of list */
    for(here = (*model)->VSRCinstances ; here ; here = here->VSRCnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
