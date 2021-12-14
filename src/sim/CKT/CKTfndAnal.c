/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTfndAnal
     *  find the given Analysis given its name and return the Analysis pointer
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "JOBdefs.h"
#include "TSKdefs.h"
#include "SPerror.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("CKTfndAnal.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:28 $")

/* ARGSUSED */
int
CKTfndAnal(ckt,analIndex,anal,name,inTask,taskName)
    GENERIC *ckt;
    int *analIndex;
    GENERIC **anal;
    IFuid name;
    GENERIC *inTask;
    IFuid taskName;
{
    TSKtask *task = (TSKtask *)inTask;
    register JOB *here;

    for (here = ((TSKtask *)task)->jobs;here;here = here->JOBnextJob) {
        if(strcmp(here->JOBname,name)==0) {
            if(anal) *anal = (GENERIC *)here;
            return(OK);
        }
    }
    return(E_NOTFOUND);
}
