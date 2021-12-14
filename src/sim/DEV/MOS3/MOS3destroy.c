/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS3defs.h"
#include "suffix.h"

RCSID("MOS3destroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:25 $")

void
MOS3destroy(inModel)
    GENmodel **inModel;
{
    MOS3model **model = (MOS3model **)inModel;
    MOS3instance *here;
    MOS3instance *prev = NULL;
    MOS3model *mod = *model;
    MOS3model *oldmod = NULL;

    for( ; mod ; mod = mod->MOS3nextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (MOS3instance *)NULL;
        for(here = mod->MOS3instances ; here ; here = here->MOS3nextInstance) {
            if(prev){
          if(prev->MOS3sens) FREE(prev->MOS3sens);
          FREE(prev);
            }
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
