/*
 * Copyright (c) 1985 S. Hwang
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MESmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:05 $")

int
MESmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    MESmodel **model = (MESmodel**)inModel;
    MESmodel *modfast = (MESmodel*)kill;
    MESinstance *here;
    MESinstance *prev = NULL;
    MESmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->MESnextModel)) {
        if( (*model)->MESmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->MESnextModel; /* cut deleted device out of list */
    for(here = (*model)->MESinstances ; here ; here = here->MESnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
