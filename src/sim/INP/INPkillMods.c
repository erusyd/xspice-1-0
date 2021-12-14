/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "INPdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("INPkillMods.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:53 $")

extern INPmodel *modtab;

void
INPkillMods()
{
    INPmodel *modtmp;
    INPmodel *prev = NULL;

    for (modtmp = modtab;modtmp != (INPmodel *)NULL;modtmp =
            modtmp->INPnextModel) {
        if(prev) FREE(prev);
        prev = modtmp;
    }
    if(prev) FREE(prev);
    modtab = (INPmodel *)NULL;
}

