/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MUTdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:11 $")

#ifdef MUTUAL
int
MUTdelete(inModel,name,kill)
    GENmodel *inModel;
    IFuid name;
    GENinstance **kill;
{
    MUTmodel *model = (MUTmodel*)inModel;
    MUTinstance **fast = (MUTinstance**)kill;
    MUTinstance **prev = NULL;
    MUTinstance *here;

    for( ; model ; model = model->MUTnextModel) {
        prev = &(model->MUTinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->MUTname == name || (fast && here==*fast) ) {
                *prev= here->MUTnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->MUTnextInstance);
        }
    }
    return(E_NODEV);
}
#endif /*MUTUAL*/
