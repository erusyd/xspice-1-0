/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "DIOdefs.h"
#include "suffix.h"

RCSID("DIOdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:35 $")

void
DIOdestroy(inModel)
    GENmodel **inModel;
{
    DIOmodel **model = (DIOmodel**)inModel;
    DIOinstance *here;
    DIOinstance *prev = NULL;
    DIOmodel *mod = *model;
    DIOmodel *oldmod = NULL;

    for( ; mod ; mod = mod->DIOnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (DIOinstance *)NULL;
        for(here = mod->DIOinstances ; here ; here = here->DIOnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
