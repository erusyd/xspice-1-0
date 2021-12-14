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

RCSID("ASRCmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:28 $")

int
ASRCmDelete(modList,modname,killModel)
    GENmodel **modList;
    IFuid modname;
    GENmodel *killModel;

{

    register ASRCmodel **model = (ASRCmodel**)modList;
    register ASRCmodel *modfast = (ASRCmodel*)killModel;
    register ASRCinstance *here;
    register ASRCinstance *prev = NULL;
    register ASRCmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->ASRCnextModel)) {
        if( (*model)->ASRCmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->ASRCnextModel; /* cut deleted device out of list */
    for(here = (*model)->ASRCinstances ; here ; here = here->ASRCnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
