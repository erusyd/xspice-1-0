/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "ISRCdefs.h"
#include "suffix.h"

RCSID("ISRCdestroy.c $Revision: 1.1 $ on $Date: 91/07/11 11:06:28 $")

void
ISRCdestroy(inModel)
    GENmodel **inModel;
{
    ISRCmodel **model = (ISRCmodel**)inModel;
    ISRCinstance *here;
    ISRCinstance *prev = NULL;
    ISRCmodel *mod = *model;
    ISRCmodel *oldmod = NULL;

    for( ; mod ; mod = mod->ISRCnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (ISRCinstance *)NULL;
        for(here = mod->ISRCinstances ; here ; here = here->ISRCnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
