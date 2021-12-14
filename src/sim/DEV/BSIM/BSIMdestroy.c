/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "BSIMdefs.h"
#include "suffix.h"

RCSID("BSIMdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:03 $")

void
BSIMdestroy(inModel)
    GENmodel **inModel;

{

    BSIMmodel **model = (BSIMmodel**)inModel;
    BSIMinstance *here;
    BSIMinstance *prev = NULL;
    BSIMmodel *mod = *model;
    BSIMmodel *oldmod = NULL;

    for( ; mod ; mod = mod->BSIMnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (BSIMinstance *)NULL;
        for(here = mod->BSIMinstances ; here ; here = here->BSIMnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
