/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MOS3defs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MOS3delete.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:24 $")

int
MOS3delete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    MOS3model *model = (MOS3model *)inModel;
    MOS3instance **fast = (MOS3instance **)inst;
    MOS3instance **prev = NULL;
    MOS3instance *here;

    for( ; model ; model = model->MOS3nextModel) {
        prev = &(model->MOS3instances);
        for(here = *prev; here ; here = *prev) {
            if(here->MOS3name == name || (fast && here==*fast) ) {
                *prev= here->MOS3nextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->MOS3nextInstance);
        }
    }
    return(E_NODEV);
}
