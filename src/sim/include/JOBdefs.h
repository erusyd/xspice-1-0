/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef JOBdef
#define JOBdef "JOBdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:15 $ "

#include "IFsim.h"

typedef struct sJOB{
    int JOBtype;                /* type of job */
    struct sJOB *JOBnextJob;    /* next job in list */
    IFuid JOBname;              /* name of this job */

} JOB;

typedef struct {
    IFanalysis public;
    int size;
    int (*(setParm))();
    int (*(askQuest))();
}SPICEanalysis;

#endif /*JOBdef*/
