/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS2defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS2delete.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:51 $")

int
MOS2delete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    MOS2model *model = (MOS2model *)inModel;
    MOS2instance **fast = (MOS2instance **)inst;
    MOS2instance **prev = NULL;
    MOS2instance *here;

    for( ; model ; model = model->MOS2nextModel) {
        prev = &(model->MOS2instances);
        for(here = *prev; here ; here = *prev) {
            if(here->MOS2name == name || (fast && here==*fast) ) {
                *prev= here->MOS2nextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->MOS2nextInstance);
        }
    }
    return(E_NODEV);
}
