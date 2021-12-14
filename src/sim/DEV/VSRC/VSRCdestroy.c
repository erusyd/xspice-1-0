/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "VSRCdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("VSRCdestroy.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:38 $")

void
VSRCdestroy(inModel)
    GENmodel **inModel;
{
    VSRCmodel **model = (VSRCmodel**)inModel;
    VSRCinstance *here;
    VSRCinstance *prev = NULL;
    VSRCmodel *mod = *model;
    VSRCmodel *oldmod = NULL;

    for( ; mod ; mod = mod->VSRCnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (VSRCinstance *)NULL;
        for(here = mod->VSRCinstances ; here ; here = here->VSRCnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
