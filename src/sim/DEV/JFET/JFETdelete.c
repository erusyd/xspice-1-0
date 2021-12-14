/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "JFETdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("JFETdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:43 $")

int
JFETdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    JFETmodel *model = (JFETmodel*)inModel;
    JFETinstance **fast = (JFETinstance**)inst;
    JFETinstance **prev = NULL;
    JFETinstance *here;

    for( ; model ; model = model->JFETnextModel) {
        prev = &(model->JFETinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->JFETname == name || (fast && here==*fast) ) {
                *prev= here->JFETnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->JFETnextInstance);
        }
    }
    return(E_NODEV);
}
