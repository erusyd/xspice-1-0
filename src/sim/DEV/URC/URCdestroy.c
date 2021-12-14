/*
 * Copyright (c) 1987 Thomas L. Quarles
 */


#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "URCdefs.h"
#include "suffix.h"

RCSID("URCdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:52 $")

void
URCdestroy(inModel)
    GENmodel **inModel;
{
    URCmodel **model = (URCmodel **)inModel;
    URCinstance *here;
    URCinstance *prev = NULL;
    URCmodel *mod = *model;
    URCmodel *oldmod = NULL;

    for( ; mod ; mod = mod->URCnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (URCinstance *)NULL;
        for(here = mod->URCinstances ; here ; here = here->URCnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
