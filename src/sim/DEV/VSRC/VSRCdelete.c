/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "VSRCdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VSRCdelete.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:36 $")

int
VSRCdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    VSRCmodel *model = (VSRCmodel *)inModel;
    VSRCinstance **fast = (VSRCinstance**)inst;
    VSRCinstance **prev = NULL;
    VSRCinstance *here;

    for( ; model ; model = model->VSRCnextModel) {
        prev = &(model->VSRCinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->VSRCname == name || (fast && here==*fast) ) {
                *prev= here->VSRCnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->VSRCnextInstance);
        }
    }
    return(E_NODEV);
}
