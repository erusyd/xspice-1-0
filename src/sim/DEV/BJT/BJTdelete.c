/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*
 * This routine deletes a BJT instance from the circuit and frees
 * the storage it was using.
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "BJTdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BJTdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:38 $")

int
BJTdelete(inModel,name,kill)
    GENmodel *inModel;
    IFuid name;
    GENinstance **kill;

{
    BJTmodel *model = (BJTmodel*)inModel;
    BJTinstance **fast = (BJTinstance**)kill;

    BJTinstance **prev = NULL;
    BJTinstance *here;

    for( ; model ; model = model->BJTnextModel) {
        prev = &(model->BJTinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->BJTname == name || (fast && here==*fast) ) {
                *prev= here->BJTnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->BJTnextInstance);
        }
    }
    return(E_NODEV);
}
