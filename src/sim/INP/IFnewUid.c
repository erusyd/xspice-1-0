/*
 * Copyright (c) 1988 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "INPdefs.h"
#include "util.h"
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CPdefs.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
#include "suffix.h"

RCSID("IFnewUid.c $Revision: 1.1 $ on $Date: 91/04/02 11:55:55 $")

int
IFnewUid(ckt,newuid,olduid,suffix,type,nodedata) 
    GENERIC *ckt;
    IFuid *newuid;
    IFuid olduid;
    char *suffix;
    int type;
    GENERIC **nodedata;
{
    char *newname;
    int error;

    if(olduid) {
        newname=(char *)
                MALLOC(sizeof(char)*(strlen(suffix)+strlen((char*)olduid)+2));
                /* 2 = '#' + '\0' */
        sprintf(newname,"%s#%s",(char*)olduid,suffix);
    } else {
        newname=(char *)MALLOC(sizeof(char)*
                (strlen(suffix)+1)); /* 1 = '\0' */
        sprintf(newname,"%s",suffix);
    }

    switch(type) {
        case UID_ANALYSIS:
        case UID_TASK:
        case UID_INSTANCE:
        case UID_OTHER:
        case UID_MODEL:
            error = INPinsert(&newname,
                    (INPtables *)ft_curckt->ci_symtab);
            if(error && error != E_EXISTS) return(error);
            *newuid = (IFuid) newname;
            break;

        case UID_SIGNAL:
            error = INPmkTerm(ckt,&newname,
                    (INPtables *)ft_curckt->ci_symtab,nodedata);
            if(error && error != E_EXISTS) return(error);
            *newuid = (IFuid) newname;
            return(error);

        default:
            return(E_BADPARM);
    }
    return(OK);
}
