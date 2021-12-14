/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef TF
#define TF "TFdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:42 $ "

#include "JOBdefs.h"
#include "TSKdefs.h"
#include "CKTdefs.h"
    /*
    /* TFdefs.h - defs for transfer function analyses */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;
    IFuid JOBname;
    CKTnode *TFoutPos;
    CKTnode *TFoutNeg;
    IFuid TFoutSrc;
    IFuid TFinSrc;
    char *TFoutName;    /* a printable name for an output v(x,y) */
    unsigned int TFoutIsV :1;
    unsigned int TFoutIsI :1;
    unsigned int TFinIsV :1;
    unsigned int TFinIsI :1;
} TFan;

#define TF_OUTPOS 1
#define TF_OUTNEG 2
#define TF_OUTSRC 3
#define TF_INSRC 4
#define TF_OUTNAME 5
extern int TFsetParm();
extern int TFaskQuest();
#endif /*TF*/
