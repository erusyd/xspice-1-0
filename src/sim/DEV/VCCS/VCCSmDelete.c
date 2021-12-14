/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "VCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCCSmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:06 $")

int
VCCSmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    VCCSmodel **model = (VCCSmodel **)inModel;
    VCCSmodel *modfast = (VCCSmodel *)kill;
    VCCSinstance *here;
    VCCSinstance *prev = NULL;
    VCCSmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->VCCSnextModel)) {
        if( (*model)->VCCSmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->VCCSnextModel; /* cut deleted device out of list */
    for(here = (*model)->VCCSinstances ; here ; here = here->VCCSnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
