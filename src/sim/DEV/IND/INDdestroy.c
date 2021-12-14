/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INDdefs.h"
#include "suffix.h"

RCSID("INDdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:00 $")

void
INDdestroy(inModel)
    GENmodel **inModel;
{
    INDmodel **model = (INDmodel**)inModel;
    INDinstance *here;
    INDinstance *prev = NULL;
    INDmodel *mod = *model;
    INDmodel *oldmod = NULL;

    for( ; mod ; mod = mod->INDnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (INDinstance *)NULL;
        for(here = mod->INDinstances ; here ; here = here->INDnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
