/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "ISRCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("ISRCmDelete.c $Revision: 1.1 $ on $Date: 91/07/11 11:06:33 $")

int
ISRCmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    ISRCmodel **model = (ISRCmodel**)inModel;
    ISRCmodel *modfast = (ISRCmodel*)kill;
    ISRCinstance *here;
    ISRCinstance *prev = NULL;
    ISRCmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->ISRCnextModel)) {
        if( (*model)->ISRCmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->ISRCnextModel; /* cut deleted device out of list */
    for(here = (*model)->ISRCinstances ; here ; here = here->ISRCnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
