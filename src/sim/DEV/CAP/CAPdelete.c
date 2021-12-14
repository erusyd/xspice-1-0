/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CAPdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CAPdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:25 $")

int
CAPdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;

{

    CAPinstance **fast = (CAPinstance**)inst;
    CAPmodel *model = (CAPmodel*)inModel;
    CAPinstance **prev = NULL;
    CAPinstance *here;

    for( ; model ; model = model->CAPnextModel) {
        prev = &(model->CAPinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->CAPname == name || (fast && here==*fast) ) {
                *prev= here->CAPnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->CAPnextInstance);
        }
    }
    return(E_NODEV);
}
