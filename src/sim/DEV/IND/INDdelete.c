/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("INDdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:59 $")

int
INDdelete(inModel,name,kill)
    GENmodel *inModel;
    IFuid name;
    GENinstance **kill;
{
    INDmodel *model = (INDmodel*)inModel;
    INDinstance **fast = (INDinstance**)kill;
    INDinstance **prev = NULL;
    INDinstance *here;

    for( ; model ; model = model->INDnextModel) {
        prev = &(model->INDinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->INDname == name || (fast && here==*fast) ) {
                *prev= here->INDnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->INDnextInstance);
        }
    }
    return(E_NODEV);
}
