/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef TRAN
#define TRAN "TRANdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:43 $ "

#include "JOBdefs.h"
#include "TSKdefs.h"
    /*
    /* TRANdefs.h - defs for transient analyses */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;
    char *JOBname;
    double TRANfinalTime;
    double TRANstep;
    double TRANmaxStep;
    double TRANinitTime;
    long TRANmode;
    GENERIC * TRANplot;
} TRANan;

#define TRAN_TSTART 1
#define TRAN_TSTOP 2
#define TRAN_TSTEP 3
#define TRAN_TMAX 4
#define TRAN_UIC 5
extern int TRANsetParm();
extern int TRANaskQuest();
#endif /*TRAN*/
