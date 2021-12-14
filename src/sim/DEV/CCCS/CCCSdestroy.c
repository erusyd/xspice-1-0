/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CCCSdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CCCSdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:47 $")

void
CCCSdestroy(inModel)
    GENmodel **inModel;

{
    CCCSmodel **model = (CCCSmodel**)inModel;
    CCCSinstance *here;
    CCCSinstance *prev = NULL;
    CCCSmodel *mod = *model;
    CCCSmodel *oldmod = NULL;

    for( ; mod ; mod = mod->CCCSnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (CCCSinstance *)NULL;
        for(here = mod->CCCSinstances ; here ; here = here->CCCSnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
