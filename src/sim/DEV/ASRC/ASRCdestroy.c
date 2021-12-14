/*
 * Copyright (c) 1987 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

#include "prefix.h"
#include <stdio.h>
#include "ASRCdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("ASRCdestroy.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:26 $")

void
ASRCdestroy(model)
    GENmodel **model;

{

    ASRCmodel **start = (ASRCmodel**)model; /* starting model */
    ASRCinstance *here;     /* current instance */
    ASRCinstance *prev = NULL;  /* previous instance */
    ASRCmodel *mod = *start;    /* current model */
    ASRCmodel *oldmod = NULL;   /* previous model */

    for( ; mod ; mod = mod->ASRCnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (ASRCinstance *)NULL;
        for(here = mod->ASRCinstances ; here ; here = here->ASRCnextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
