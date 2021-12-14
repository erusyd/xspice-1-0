/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef ASRC
#define ASRC "ASRCdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:25:09 $ "

#include "CKTdefs.h"
#include "IFsim.h"
#include "complex.h"

        /*
         * structures to describe Arbitrary sources
         */

/* information to describe a single instance */

typedef struct sASRCinstance {
    struct sASRCmodel *ARRCmodPtr;  /* backpointer to model */
    struct sASRCinstance *ASRCnextInstance;  /* pointer to next instance of 
                                              *current model*/
    IFuid ASRCname; /* pointer to character string naming this instance */
    int ASRCposNode;    /* number of positive node of source */
    int ASRCnegNode;    /* number of negative node of source */
    int ASRCtype;   /* Whether source is voltage or current */
    int ASRCbranch;     /* number of branch equation added for v source */
    IFparseTree *ASRCtree; /* The parse tree */
    double **ASRCposptr;  /* pointer to pointers of the elements
               * in the sparce matrix */
    double ASRCprev_value; /* Previous value for the convergence test */
    double *ASRCacValues;  /* Store rhs and derivatives for ac anal */
    int ASRCcont_br;    /* Temporary store for controlling current branch */
    int ASRCstates; /* state info */
#define ASRCvOld ASRCstates
#define ASRCcontVOld ASRCstates + 1

} ASRCinstance ;


/* per model data */

typedef struct sASRCmodel {       /* model structure for a source */
    int ASRCmodType;    /* type index of this device */
    struct sASRCmodel *ASRCnextModel;   /* pointer to next possible model 
                                         *in linked list */
    ASRCinstance * ASRCinstances;    /* pointer to list of instances 
                                      * that have this model */
    IFuid ASRCmodName;       /* pointer to character string naming this model */
} ASRCmodel;

/* device parameters */
#define ASRC_VOLTAGE 1
#define ASRC_CURRENT 2
#define ASRC_POS_NODE 3
#define ASRC_NEG_NODE 4
#define ASRC_PARSE_TREE 5

/* model parameters */

/* device questions */

/* model questions */

#ifdef __STDC__
extern int ASRCask(CKTcircuit*,GENinstance *,int,IFvalue *,IFvalue*);
extern int ASRCconvTest(GENmodel *,CKTcircuit*);
extern int ASRCdelete(GENmodel *,IFuid,GENinstance **);
extern void ASRCdestroy(GENmodel**);
extern int ASRCfindBr(CKTcircuit *,GENmodel *,IFuid);
extern int ASRCload(GENmodel *,CKTcircuit*);
extern int ASRCmDelete(GENmodel**,IFuid,GENmodel*);
extern int ASRCparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int ASRCpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int ASRCacLoad(GENmodel*,CKTcircuit*);
extern int ASRCsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
#else /* stdc */
extern int ASRCask();
extern int ASRCconvTest();
extern int ASRCdelete();
extern void ASRCdestroy();
extern int ASRCfindBr();
extern int ASRCload();
extern int ASRCmDelete();
extern int ASRCparam();
extern int ASRCpzLoad();
extern int ASRCacLoad();
extern int ASRCsetup();
#endif /* stdc */

#endif /*ASRC*/
