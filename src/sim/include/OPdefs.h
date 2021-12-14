
/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef DCOP
#define DCOP "OPdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:33 $ "

#include "JOBdefs.h"
#include "TSKdefs.h"
    /*
     * structures used to describe D.C. operationg point analyses to
     * be performed.
     */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;
    char *JOBname;
} OP;

extern int DCOsetParm();
extern int DCOaskQuest();
#endif /*DCOP*/
