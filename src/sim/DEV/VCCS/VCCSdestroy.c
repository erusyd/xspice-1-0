/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "VCCSdefs.h"
#include "suffix.h"

RCSID("VCCSdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:05 $")

void
VCCSdestroy(inModel)
    GENmodel **inModel;
{
    VCCSmodel **model = (VCCSmodel**)inModel;
    VCCSinstance *here;
    VCCSinstance *prev = NULL;
    VCCSmodel *mod = *model;
    VCCSmodel *oldmod = NULL;

    for( ; mod ; mod = mod->VCCSnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (VCCSinstance *)NULL;
        for(here = mod->VCCSinstances ; here ; here = here->VCCSnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
