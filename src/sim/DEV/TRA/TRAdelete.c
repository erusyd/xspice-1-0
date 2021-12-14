/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "TRAdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("TRAdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:37 $")

int
TRAdelete(inModel,name,kill)
    GENmodel *inModel;
    IFuid name;
    GENinstance **kill;
{
    TRAinstance **fast = (TRAinstance **)kill;
    TRAmodel *model = (TRAmodel *)inModel;
    TRAinstance **prev = NULL;
    TRAinstance *here;

    for( ; model ; model = model->TRAnextModel) {
        prev = &(model->TRAinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->TRAname == name || (fast && here==*fast) ) {
                *prev= here->TRAnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->TRAnextInstance);
        }
    }
    return(E_NODEV);
}
