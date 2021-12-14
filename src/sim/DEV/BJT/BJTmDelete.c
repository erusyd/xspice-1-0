/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*
 * This routine deletes a BJT model from the circuit and frees
 * the storage it was using.
 * returns an error if the model has instances
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "BJTdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("BJTmDelete.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:42 $")

int
BJTmDelete(inModels,modname,kill)
    GENmodel **inModels;
    IFuid modname;
    GENmodel *kill;

{
    BJTmodel **model = (BJTmodel**)inModels;
    BJTmodel *modfast = (BJTmodel*)kill;

    BJTmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->BJTnextModel)) {
        if( (*model)->BJTmodName == modname || 
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    if( (*model)->BJTinstances ) return(E_NOTEMPTY);
    *oldmod = (*model)->BJTnextModel; /* cut deleted device out of list */
    FREE(*model);
    return(OK);

}
