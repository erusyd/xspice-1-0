/*
 * Copyright (c) 1987 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

#include "prefix.h"
#include <stdio.h>
#include "ASRCdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("ASRCdelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:25 $")

int
ASRCdelete(model,name,fast)
    GENmodel *model;
    IFuid name;
    GENinstance **fast;

{
    register ASRCinstance **instPtr = (ASRCinstance**)fast;
    ASRCmodel *modPtr = (ASRCmodel*)model;

    ASRCinstance **prev = NULL;
    ASRCinstance *here;

    for( ; modPtr ; modPtr = modPtr->ASRCnextModel) {
        prev = &(modPtr->ASRCinstances);
        for(here = *prev; here ; here = *prev) {
            if(here->ASRCname == name || (instPtr && here==*instPtr) ) {
                *prev= here->ASRCnextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->ASRCnextInstance);
        }
    }
    return(E_NODEV);
}
