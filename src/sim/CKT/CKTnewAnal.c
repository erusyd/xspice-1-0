/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "TSKdefs.h"
#include "JOBdefs.h"
#include "IFsim.h"
#include "util.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "suffix.h"

RCSID("CKTnewAnal.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:54 $")

extern SPICEanalysis *analInfo[];

/* ARGSUSED */
int
CKTnewAnal(ckt,type,name,analPtr,taskPtr)
    GENERIC *ckt;
    int type;
    IFuid name;
    GENERIC **analPtr;
    GENERIC *taskPtr;
{
    if(type==0) {
        /* special case for analysis type 0 == option card */
        *analPtr=taskPtr; /* pointer to the task itself */
        (*(JOB **)analPtr)->JOBname = name;
        (*(JOB **)analPtr)->JOBtype = type;
        return(OK); /* doesn't need to be created */
    }
    *analPtr = (GENERIC *)MALLOC(analInfo[type]->size);
    if(*analPtr==NULL) return(E_NOMEM);
    (*(JOB **)analPtr)->JOBname = name;
    (*(JOB **)analPtr)->JOBtype = type;
    (*(JOB **)analPtr)->JOBnextJob = ((TSKtask *)taskPtr)->jobs;
    ((TSKtask *)taskPtr)->jobs = (JOB *)*analPtr;
    return(OK);
}
