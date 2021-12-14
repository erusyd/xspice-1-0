/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "VCVSdefs.h"
#include "suffix.h"

RCSID("VCVSdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:24 $")

void
VCVSdestroy(inModel)
    GENmodel **inModel;
{
    VCVSmodel **model = (VCVSmodel **)inModel;
    VCVSinstance *here;
    VCVSinstance *prev = NULL;
    VCVSmodel *mod = *model;
    VCVSmodel *oldmod = NULL;

    for( ; mod ; mod = mod->VCVSnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (VCVSinstance *)NULL;
        for(here = mod->VCVSinstances ; here ; here = here->VCVSnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
