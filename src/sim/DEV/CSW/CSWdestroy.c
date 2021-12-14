/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "CSWdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CSWdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:19 $")

void
CSWdestroy(inModel)
    GENmodel **inModel;
{
    CSWmodel **model = (CSWmodel**)inModel;
    CSWinstance *here;
    CSWinstance *prev = NULL;
    CSWmodel *mod = *model;
    CSWmodel *oldmod = NULL;

    for( ; mod ; mod = mod->CSWnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (CSWinstance *)NULL;
        for(here = mod->CSWinstances ; here ; here = here->CSWnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
