/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MUTmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:12 $")

#ifdef MUTUAL
int
MUTmDelete(inModel,modname,kill)
    GENmodel **inModel;
    IFuid modname;
    GENmodel *kill;
{
    MUTmodel **model = (MUTmodel**)inModel;
    MUTmodel *modfast = (MUTmodel*)kill;
    MUTinstance *here;
    MUTinstance *prev = NULL;
    MUTmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->MUTnextModel)) {
        if( (*model)->MUTmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->MUTnextModel; /* cut deleted device out of list */
    for(here = (*model)->MUTinstances ; here ; here = here->MUTnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
#endif /* MUTUAL */
