/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef ACDEFS
#define ACDEFS "ACdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:25:07 $ "

#include "JOBdefs.h"

    /* structure used to describe an AC analysis to be performed */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;    /* pointer to next thing to do */
    char *JOBname;      /* name of this job */
    double ACstartFreq;
    double ACstopFreq;
    double ACfreqDelta; /* multiplier for decade/octave stepping, */
                        /* step for linear steps. */
    double ACsaveFreq;  /* frequency at which we left off last time*/
    int ACstepType;     /* values described below */
    int ACnumberSteps;
} ACAN;

/* available step types: */

#define DECADE 1
#define OCTAVE 2
#define LINEAR 3

#define AC_DEC 1
#define AC_OCT 2
#define AC_LIN 3
#define AC_START 4
#define AC_STOP 5
#define AC_STEPS 6

#endif /*ACDEFS*/
