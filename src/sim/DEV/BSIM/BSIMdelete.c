/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "BSIMdefs.h"
#include "SPerror.h"
#include "GENdefs.h"
#include "suffix.h"

RCSID("BSIMdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:02 $")

int
BSIMdelete(inModel,name,inInst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inInst;

{

    BSIMinstance **fast = (BSIMinstance**)inInst;
    BSIMmodel *model = (BSIMmodel*)inModel;
    BSIMinstance **prev = NULL;
    BSIMinstance *here;

    for( ; model ; model = model->BSIMnextModel) {
        prev = &(model->BSIMinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->BSIMname == name || (fast && here==*fast) ) {
                *prev= here->BSIMnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->BSIMnextInstance);
        }
    }
    return(E_NODEV);
}
