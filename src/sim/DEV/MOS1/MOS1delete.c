/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS1defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS1delete.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:22 $")

int
MOS1delete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    MOS1model *model = (MOS1model *)inModel;
    MOS1instance **fast = (MOS1instance **)inst;
    MOS1instance **prev = NULL;
    MOS1instance *here;

    for( ; model ; model = model->MOS1nextModel) {
        prev = &(model->MOS1instances);
        for(here = *prev; here ; here = *prev) {
            if(here->MOS1name == name || (fast && here==*fast) ) {
                *prev= here->MOS1nextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->MOS1nextInstance);
        }
    }
    return(E_NODEV);
}
