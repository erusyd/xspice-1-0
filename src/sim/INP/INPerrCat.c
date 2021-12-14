/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <string.h>
#include <stdio.h>
#include "INPdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("INPerrCat.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:40 $")

char *
INPerrCat(a,b)
    char *a;
    char *b;
{

    if(a != (char *)NULL) {
        if(b == (char *)NULL) { /* a valid, b null, return a */
            return(a);
        } else { /* both valid  - hard work...*/
            register char *errtmp;
            errtmp = (char *)MALLOC( (strlen(a) + strlen(b)+2)*sizeof(char));
            (void) strcpy(errtmp,a);
            (void) strcat(errtmp,"\n");
            (void) strcat(errtmp,b);
            FREE(a);
            FREE(b);
            return(errtmp);
        }
    } else { /* a null, so return b */
        return(b);
    }
}
