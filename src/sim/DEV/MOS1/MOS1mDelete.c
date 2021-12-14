/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS1defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS1mDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:28 $")

int
MOS1mDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    MOS1model **model = (MOS1model **)inModel;
    MOS1model *modfast = (MOS1model *)kill;
    MOS1instance *here;
    MOS1instance *prev = NULL;
    MOS1model **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->MOS1nextModel)) {
        if( (*model)->MOS1modName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->MOS1nextModel; /* cut deleted device out of list */
    for(here = (*model)->MOS1instances ; here ; here = here->MOS1nextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
