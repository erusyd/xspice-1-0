/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SWdefs.h"
#include "suffix.h"

RCSID("SWdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:21 $")

void
SWdestroy(inModel)
    GENmodel **inModel;
{
    SWmodel **model = (SWmodel**)inModel;
    SWinstance *here;
    SWinstance *prev = NULL;
    SWmodel *mod = *model;
    SWmodel *oldmod = NULL;

    for( ; mod ; mod = mod->SWnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (SWinstance *)NULL;
        for(here = mod->SWinstances ; here ; here = here->SWnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
