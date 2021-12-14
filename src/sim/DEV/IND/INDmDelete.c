/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("INDmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:01 $")

int
INDmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    INDmodel **model = (INDmodel**)inModel;
    INDmodel *modfast = (INDmodel*)kill;
    INDinstance *here;
    INDinstance *prev = NULL;
    INDmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->INDnextModel)) {
        if( (*model)->INDmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->INDnextModel; /* cut deleted device out of list */
    for(here = (*model)->INDinstances ; here ; here = here->INDnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
