/*
 * Copyright (c) 1987 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#include "ASRCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("ASRCparam.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:29 $")

/* ARGSUSED */
int
ASRCparam(param,value,fast,select)
    int param;
    IFvalue *value;
    GENinstance *fast;
    IFvalue *select;
{
    register ASRCinstance *here = (ASRCinstance*)fast;
    switch(param) {
        case ASRC_VOLTAGE:
            here->ASRCtype = ASRC_VOLTAGE;
        here->ASRCtree = value->tValue;
            break;
        case ASRC_CURRENT:
            here->ASRCtype = ASRC_CURRENT;
        here->ASRCtree = value->tValue;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
