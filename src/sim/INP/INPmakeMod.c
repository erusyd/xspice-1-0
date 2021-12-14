/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INPdefs.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include <string.h>
#include "suffix.h"

RCSID("INPmakeMod.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:59 $")

INPmodel *modtab;

    /* create/lookup a 'model' entry */

int
INPmakeMod(token,type,line)
    char *token;
    int type;
    card *line;
{
    register INPmodel **i;

    for (i = &modtab;*i != (INPmodel *)NULL;i = &((*i)->INPnextModel)) {
        if (strcmp((*i)->INPmodName,token) == 0) {
            return(OK);
        }
    }
    *i = (INPmodel *)MALLOC(sizeof(INPmodel));
    if(*i==NULL) return(E_NOMEM);
    (*i)->INPmodName = token;
    (*i)->INPmodType = type;
    (*i)->INPnextModel = (INPmodel *)NULL;
    (*i)->INPmodUsed = 0;
    (*i)->INPmodLine = line;
    (*i)->INPmodfast = NULL;
    return(OK);
}
