/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "TRAdefs.h"
#include "suffix.h"

RCSID("TRAdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:38 $")

void
TRAdestroy(inModel)
    GENmodel **inModel;
{
    TRAmodel **model = (TRAmodel **)inModel;
    TRAinstance *here;
    TRAinstance *prev = NULL;
    TRAmodel *mod = *model;
    TRAmodel *oldmod = NULL;

    for( ; mod ; mod = mod->TRAnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (TRAinstance *)NULL;
        for(here = mod->TRAinstances ; here ; here = here->TRAnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
