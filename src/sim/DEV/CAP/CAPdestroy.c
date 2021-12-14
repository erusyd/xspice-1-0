/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CAPdefs.h"
#include "suffix.h"

RCSID("CAPdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:26 $")

void
CAPdestroy(inModel)
    GENmodel **inModel;

{

    CAPmodel **model = (CAPmodel**)inModel;
    CAPinstance *here;
    CAPinstance *prev = NULL;
    CAPmodel *mod = *model;
    CAPmodel *oldmod = NULL;

    for( ; mod ; mod = mod->CAPnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (CAPinstance *)NULL;
        for(here = mod->CAPinstances ; here ; here = here->CAPnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
