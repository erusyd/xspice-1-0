/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CCVSdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CCVSdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:03 $")

void
CCVSdestroy(inModel)
    GENmodel **inModel;
{
    CCVSmodel **model = (CCVSmodel**)inModel;
    CCVSinstance *here;
    CCVSinstance *prev = NULL;
    CCVSmodel *mod = *model;
    CCVSmodel *oldmod = NULL;

    for( ; mod ; mod = mod->CCVSnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (CCVSinstance *)NULL;
        for(here = mod->CCVSinstances ; here ; here = here->CCVSnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
