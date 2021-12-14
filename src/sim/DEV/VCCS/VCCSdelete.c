/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "VCCSdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCCSdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:04 $")

int
VCCSdelete(inModel,name,inst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inst;
{
    VCCSmodel *model = (VCCSmodel *)inModel;
    VCCSinstance **fast = (VCCSinstance**)inst;
    VCCSinstance **prev = NULL;
    VCCSinstance *here;

    for( ; model ; model = model->VCCSnextModel) {
        prev = &(model->VCCSinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->VCCSname == name || (fast && here==*fast) ) {
                *prev= here->VCCSnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->VCCSnextInstance);
        }
    }
    return(E_NODEV);
}
