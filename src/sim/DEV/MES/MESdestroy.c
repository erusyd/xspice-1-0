/*
 * Copyright (c) 1985 S. Hwang
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MESdefs.h"
#include "suffix.h"

RCSID("MESdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:02 $")

void
MESdestroy(inModel)
    GENmodel **inModel;
{
    MESmodel **model = (MESmodel**)inModel;
    MESinstance *here;
    MESinstance *prev = NULL;
    MESmodel *mod = *model;
    MESmodel *oldmod = NULL;

    for( ; mod ; mod = mod->MESnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (MESinstance *)NULL;
        for(here = mod->MESinstances ; here ; here = here->MESnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
