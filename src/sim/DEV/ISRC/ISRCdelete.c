/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "ISRCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("ISRCdelete.c $Revision: 1.1 $ on $Date: 91/07/11 11:06:26 $")

int
ISRCdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    ISRCmodel *model = (ISRCmodel*)inModel;
    ISRCinstance **fast = (ISRCinstance**)inst;
    ISRCinstance **prev = NULL;
    ISRCinstance *here;

    for( ; model ; model = model->ISRCnextModel) {
        prev = &(model->ISRCinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->ISRCname == name || (fast && here==*fast) ) {
                *prev= here->ISRCnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->ISRCnextInstance);
        }
    }
    return(E_NODEV);
}
