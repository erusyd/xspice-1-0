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

RCSID("CKTdelTask.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:18 $")

/* ARGSUSED */
int
CKTdelTask(ckt,task)
    GENERIC *ckt;
    GENERIC *task;
{
    JOB *job;
    JOB *old=NULL;
    for(job = ((TSKtask*)task)->jobs; job; job=job->JOBnextJob){
        if(old) FREE(old);
        old=job;
    }
    if(old)FREE(old);
    FREE(task);
    return(OK);
}
