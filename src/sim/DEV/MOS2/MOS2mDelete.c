/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS2defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS2mDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:56 $")

int
MOS2mDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    MOS2model **model = (MOS2model **)inModel;
    MOS2model *modfast = (MOS2model *)kill;
    MOS2instance *here;
    MOS2instance *prev = NULL;
    MOS2model **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->MOS2nextModel)) {
        if( (*model)->MOS2modName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->MOS2nextModel; /* cut deleted device out of list */
    for(here = (*model)->MOS2instances ; here ; here = here->MOS2nextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
