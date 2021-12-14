/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*  look up the 'type' in the device description struct and return the
 *  appropriate index for the device found, or -1 for not found 
 */

#include "prefix.h"
#include <string.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "suffix.h"

RCSID("CKTtypelook.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:30 $")

extern SPICEdev *DEVices[];

int
CKTtypelook(type)
    char *type;
{

    int i;
    for(i=0;i<DEVmaxnum;i++) {
        if(strcmp(type,(*DEVices[i]).DEVpublic.name)==0) {
            /*found the device - return it */
            return(i);
        }
    }
    return(-1);
}

