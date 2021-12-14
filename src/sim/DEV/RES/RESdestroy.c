/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "RESdefs.h"
#include "suffix.h"

RCSID("RESdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:54 $")

void
RESdestroy(inModel)
    GENmodel **inModel;
{
    RESmodel **model = (RESmodel **)inModel;
    RESinstance *here;
    RESinstance *prev = NULL;
    RESmodel *mod = *model;
    RESmodel *oldmod = NULL;

    for( ; mod ; mod = mod->RESnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (RESinstance *)NULL;
        for(here = mod->RESinstances ; here ; here = here->RESnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
