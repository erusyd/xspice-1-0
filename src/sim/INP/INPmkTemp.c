/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <string.h>
#include "INPdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("INPmkTemp.c $Revision: 1.1 $ on $Date: 91/04/02 11:57:01 $")

char *
INPmkTemp(string)
char *string;
{
int len;
char *temp;
    
    len = strlen(string);
    temp = MALLOC(len+1);
    if(temp!=(char *)NULL) (void)strcpy(temp,string);
    return(temp);

}
