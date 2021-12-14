/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS2defs.h"
#include "suffix.h"

RCSID("MOS2destroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:52 $")

void
MOS2destroy(inModel)
    GENmodel **inModel;
{
    MOS2model **model = (MOS2model **)inModel;
    MOS2instance *here;
    MOS2instance *prev = NULL;
    MOS2model *mod = *model;
    MOS2model *oldmod = NULL;

    for( ; mod ; mod = mod->MOS2nextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (MOS2instance *)NULL;
        for(here = mod->MOS2instances ; here ; here = here->MOS2nextInstance) {
            if(prev){
          if(prev->MOS2sens) FREE(prev->MOS2sens);
          FREE(prev);
        }
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
