/*
 * Copyright (c) 1985 S. Hwang
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "MESdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MESdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:44:01 $")

int
MESdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    MESmodel *model = (MESmodel*)inModel;
    MESinstance **fast = (MESinstance**)inst;
    MESinstance **prev = NULL;
    MESinstance *here;

    for( ; model ; model = model->MESnextModel) {
        prev = &(model->MESinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->MESname == name || (fast && here==*fast) ) {
                *prev= here->MESnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->MESnextInstance);
        }
    }
    return(E_NODEV);
}
