/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "VCVSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:23 $")

int
VCVSdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    VCVSmodel *model = (VCVSmodel *)inModel;
    VCVSinstance **fast = (VCVSinstance**)inst;
    VCVSinstance **prev = NULL;
    VCVSinstance *here;

    for( ; model ; model = model->VCVSnextModel) {
        prev = &(model->VCVSinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->VCVSname == name || (fast && here==*fast) ) {
                *prev= here->VCVSnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->VCVSnextInstance);
        }
    }
    return(E_NODEV);
}
