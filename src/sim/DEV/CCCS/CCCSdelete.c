/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CCCSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCCSdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:46 $")

int
CCCSdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;

{

    CCCSmodel *model = (CCCSmodel*)inModel;
    CCCSinstance **fast = (CCCSinstance**)inst;
    CCCSinstance **prev = NULL;
    CCCSinstance *here;

    for( ; model ; model = model->CCCSnextModel) {
        prev = &(model->CCCSinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->CCCSname == name || (fast && here==*fast) ) {
                *prev= here->CCCSnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->CCCSnextInstance);
        }
    }
    return(E_NODEV);
}
