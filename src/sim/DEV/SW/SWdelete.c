/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SWdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SWdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:20 $")

int
SWdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    SWmodel *model = (SWmodel *)inModel;
    SWinstance **fast = (SWinstance **)inst;
    SWinstance **prev = NULL;
    SWinstance *here;

    for( ; model ; model = model->SWnextModel) {
        prev = &(model->SWinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->SWname == name || (fast && here==*fast) ) {
                *prev= here->SWnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->SWnextInstance);
        }
    }
    return(E_NODEV);
}
