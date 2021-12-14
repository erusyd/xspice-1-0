/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS1defs.h"
#include "suffix.h"

RCSID("MOS1destroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:23 $")

void
MOS1destroy(inModel)
    GENmodel **inModel;
{
    MOS1model **model = (MOS1model**)inModel;
    MOS1instance *here;
    MOS1instance *prev = NULL;
    MOS1model *mod = *model;
    MOS1model *oldmod = NULL;

    for( ; mod ; mod = mod->MOS1nextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (MOS1instance *)NULL;
        for(here = mod->MOS1instances ; here ; here = here->MOS1nextInstance) {
            if(prev){
                if(prev->MOS1sens) FREE(prev->MOS1sens); 
                FREE(prev);
            }
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
