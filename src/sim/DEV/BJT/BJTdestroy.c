/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*
 * This routine deletes all BJTs from the circuit and frees
 * all storage they were using.
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "BJTdefs.h"
#include "suffix.h"

RCSID("BJTdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:39 $")

void
BJTdestroy(inModel)
    GENmodel **inModel;

{

    BJTmodel **model = (BJTmodel**)inModel;
    BJTinstance *here;
    BJTinstance *prev = NULL;
    BJTmodel *mod = *model;
    BJTmodel *oldmod = NULL;

    for( ; mod ; mod = mod->BJTnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (BJTinstance *)NULL;
        for(here = mod->BJTinstances ; here ; here = here->BJTnextInstance) {
            if(prev){
                if(prev->BJTsens) FREE(prev->BJTsens);
                FREE(prev);
            }
            prev = here;
        }
        if(prev){
            if(prev->BJTsens) FREE(prev->BJTsens);
            FREE(prev);
        }
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
