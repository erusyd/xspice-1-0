/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CCVSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CCVSdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:02 $")

int
CCVSdelete(inModel,name,kill)
    GENmodel *inModel;
    IFuid name;
    GENinstance **kill;

{
    CCVSmodel *model = (CCVSmodel*)inModel;
    CCVSinstance **fast = (CCVSinstance**)kill;
    CCVSinstance **prev = NULL;
    CCVSinstance *here;

    for( ; model ; model = model->CCVSnextModel) {
        prev = &(model->CCVSinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->CCVSname == name || (fast && here==*fast) ) {
                *prev= here->CCVSnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->CCVSnextInstance);
        }
    }
    return(E_NODEV);
}
