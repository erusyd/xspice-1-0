/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "INPdefs.h"
#include "util.h"
#include <string.h>
#include "suffix.h"

RCSID("INPlookMod.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:57 $")

extern INPmodel *modtab;

/*ARGSUSED*/
int
INPlookMod(name)
    char *name;
{
    register INPmodel **i;

    for (i = &modtab;*i != (INPmodel *)NULL;i = &((*i)->INPnextModel)) {
        if (strcmp((*i)->INPmodName,name) == 0) {
            /* found the model in question - return true */
            return(1);
        }
    }
    /* didn't find model - return false */
    return(0);
}

