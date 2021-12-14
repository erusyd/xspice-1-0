/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INDdefs.h"
#include "suffix.h"

RCSID("MUTdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:12 $")

#ifdef MUTUAL
void
MUTdestroy(inModel)
    GENmodel **inModel;
{
    MUTmodel **model = (MUTmodel**)inModel;
    MUTinstance *here;
    MUTinstance *prev = NULL;
    MUTmodel *mod = *model;
    MUTmodel *oldmod = NULL;

    for( ; mod ; mod = mod->MUTnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (MUTinstance *)NULL;
        for(here = mod->MUTinstances ; here ; here = here->MUTnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
#endif /* MUTUAL */
