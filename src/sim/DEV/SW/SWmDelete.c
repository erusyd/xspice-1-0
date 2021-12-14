/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "SWdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SWmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:23 $")

int
SWmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    SWmodel **model = (SWmodel **)inModel;
    SWmodel *modfast = (SWmodel *)kill;
    SWinstance *here;
    SWinstance *prev = NULL;
    SWmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->SWnextModel)) {
        if( (*model)->SWmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->SWnextModel; /* cut deleted device out of list */
    for(here = (*model)->SWinstances ; here ; here = here->SWnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
