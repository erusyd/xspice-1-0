/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <ctype.h>
#include "INPdefs.h"
#include "suffix.h"

RCSID("INPcaseFix.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:32 $")

void
INPcaseFix(string)
    register char *string;
{

    while(*string) {
        if(isupper(*string)) {
            *string = tolower(*string);
        }
        string++;
    }
}
