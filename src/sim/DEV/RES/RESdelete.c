/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "RESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("RESdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:45:53 $")

int
RESdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    RESmodel *model = (RESmodel *)inModel;
    RESinstance **fast = (RESinstance **)inst;
    RESinstance **prev = NULL;
    RESinstance *here;

    for( ; model ; model = model->RESnextModel) {
        prev = &(model->RESinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->RESname == name || (fast && here==*fast) ) {
                *prev= here->RESnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->RESnextInstance);
        }
    }
    return(E_NODEV);
}
