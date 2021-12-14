/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "CSWdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CSWmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:21 $")

int
CSWmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    CSWmodel **model = (CSWmodel**)inModel;
    CSWmodel *modfast = (CSWmodel*)kill;
    CSWinstance *here;
    CSWinstance *prev = NULL;
    CSWmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->CSWnextModel)) {
        if( (*model)->CSWmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->CSWnextModel; /* cut deleted device out of list */
    for(here = (*model)->CSWinstances ; here ; here = here->CSWnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
