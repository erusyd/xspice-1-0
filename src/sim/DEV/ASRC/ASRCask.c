/*
 * Copyright (c) 1987 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

/*
 * This routine gives access to the internal device parameters
 * of Current Controlled Voltage Source
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "ASRCdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("ASRCask.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:24 $")

/* ARGSUSED */
int
ASRCask(ckt,instPtr,which,value,select)
    CKTcircuit *ckt;
    GENinstance *instPtr;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    register ASRCinstance *here = (ASRCinstance*)instPtr;

    switch(which) {
        case ASRC_CURRENT:
            value->tValue = here->ASRCtype == ASRC_CURRENT ? 
                    here->ASRCtree : NULL;
            return (OK);
        case ASRC_VOLTAGE:
            value->tValue = here->ASRCtype == ASRC_VOLTAGE ? 
                    here->ASRCtree : NULL;
            return (OK);
        case ASRC_POS_NODE:
            value->iValue = here->ASRCposNode;
            return (OK);
        case ASRC_NEG_NODE:
            value->iValue = here->ASRCnegNode;
            return (OK);
        default:
            return (E_BADPARM);
    }
    /* NOTREACHED */
}
