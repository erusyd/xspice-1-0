/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/*  look up the 'type' in the device description struct and return the
 *  appropriate index for the device found, or -1 for not found 
 */

#include "prefix.h"
#include <string.h>
#include "INPdefs.h"
#include "CPdefs.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
#include "IFsim.h"
#include "suffix.h"

RCSID("INPtypelook.c $Revision: 1.1 $ on $Date: 91/04/02 11:57:17 $")

int
INPtypelook(type)
    char *type;
{

    int i;
    for(i=0;i<ft_sim->numDevices;i++) {
        if(strcmp(type,(*(ft_sim->devices)[i]).name)==0) {
            /*found the device - return it */
            return(i);
        }
    }
    return(-1);
}

