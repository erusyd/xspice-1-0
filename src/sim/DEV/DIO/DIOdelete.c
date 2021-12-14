/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:35 $")

int
DIOdelete(inModel,name,kill)
    GENmodel *inModel;
    IFuid name;
    GENinstance **kill;
{
    DIOmodel *model = (DIOmodel*)inModel;
    DIOinstance **fast = (DIOinstance**)kill;
    DIOinstance **prev = NULL;
    DIOinstance *here;

    for( ; model ; model = model->DIOnextModel) {
        prev = &(model->DIOinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->DIOname == name || (fast && here==*fast) ) {
                *prev= here->DIOnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->DIOnextInstance);
        }
    }
    return(E_NODEV);
}
