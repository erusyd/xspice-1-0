/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "VCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:26 $")

int
VCVSmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    VCVSmodel **model = (VCVSmodel**)inModel;
    VCVSmodel *modfast = (VCVSmodel *)kill;
    VCVSinstance *here;
    VCVSinstance *prev = NULL;
    VCVSmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->VCVSnextModel)) {
        if( (*model)->VCVSmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->VCVSnextModel; /* cut deleted device out of list */
    for(here = (*model)->VCVSinstances ; here ; here = here->VCVSnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
