/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "CSWdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CSWdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:19 $")

int
CSWdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;

{
    CSWmodel *model = (CSWmodel*)inModel;
    CSWinstance **fast = (CSWinstance**)inst;
    CSWinstance **prev = NULL;
    CSWinstance *here;

    for( ; model ; model = model->CSWnextModel) {
        prev = &(model->CSWinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->CSWname == name || (fast && here==*fast) ) {
                *prev= here->CSWnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->CSWnextInstance);
        }
    }
    return(E_NODEV);
}
