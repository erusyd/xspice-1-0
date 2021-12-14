/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CKTdefs.h"
#include "OPdefs.h"
#include "suffix.h"

RCSID("DCOsetParm.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:32 $")

/* ARGSUSED */
int 
DCOsetParm(ckt,anal,which,value)
    CKTcircuit  *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    default:
        return(E_BADPARM);
    }
    /* NOTREACHED */ /* TEMPORARY until cases get added */
    return(OK);
}


/*static IFparm DCOparms[] = {
};*/

SPICEanalysis DCOinfo  = {
    { 
        "DCOP",
        "D.C. Operating point analysis",

        0,
        NULL,
    },
    sizeof(OP),
    DCOsetParm,
    DCOaskQuest
};
