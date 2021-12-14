/*
 * Copyright (c) 1985  Mani B. Srivastava
 */
#ifndef PZDEFS
#define PZDEFS "PZdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:34 $ "

    /* structure used to describe an PZ analysis to be performed */

#include "JOBdefs.h"

typedef struct sroot {
    double real;
    double imag;
    struct sroot *next;
} root;
typedef struct {
    int JOBtype;
    JOB *JOBnextJob;
    IFuid JOBname;
    int PZnodeI ;
    int PZnodeG ;
    int PZnodeJ ;
    int PZnodeK ;
    int PZdiffJK ;
    int PZdiffIG ;
    int PZflagVI ;
    int PZflagPZ ;
    int PZnumswaps ;
    root* PZpoleList;
    root* PZzeroList;
    double *PZJK_Jptr;
    double *PZJK_Kptr;
    double *PZJK_JKptr;
    double *PZIG_Iptr;
    double *PZIG_Gptr;
    double *PZIG_IGptr;
} PZAN;

#define PZ_NODEI 1
#define PZ_NODEG 2
#define PZ_NODEJ 3
#define PZ_NODEK 4
#define PZ_V 5
#define PZ_I 6
#define PZ_POL 7
#define PZ_ZER 8
#define PZ_PZ 9

#endif /*PZDEFS*/
