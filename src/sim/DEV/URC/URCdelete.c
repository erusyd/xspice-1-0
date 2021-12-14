/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "URCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("URCdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:51 $")

int
URCdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    URCmodel *model = (URCmodel *)inModel;
    URCinstance **fast = (URCinstance**)inst;
    URCinstance **prev = NULL;
    URCinstance *here;

    for( ; model ; model = model->URCnextModel) {
        prev = &(model->URCinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->URCname == name || (fast && here==*fast) ) {
                *prev= here->URCnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->URCnextInstance);
        }
    }
    return(E_NODEV);
}
