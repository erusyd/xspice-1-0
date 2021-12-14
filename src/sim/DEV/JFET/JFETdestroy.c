/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "JFETdefs.h"
#include "suffix.h"

RCSID("JFETdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:44 $")

void
JFETdestroy(inModel)
    GENmodel **inModel;
{
    JFETmodel **model = (JFETmodel**)inModel;
    JFETinstance *here;
    JFETinstance *prev = NULL;
    JFETmodel *mod = *model;
    JFETmodel *oldmod = NULL;

    for( ; mod ; mod = mod->JFETnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (JFETinstance *)NULL;
        for(here = mod->JFETinstances ; here ; here = here->JFETnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
