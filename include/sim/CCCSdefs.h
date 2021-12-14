/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef CCCS
#define CCCS "CCCSdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:25:15 $ "

#include "IFsim.h"
#include "GENdefs.h"
#include "complex.h"
#include "CKTdefs.h"

    /* structures used to describe Current Controlled Current Sources */

/* information needed for each instance */

typedef struct sCCCSinstance {
    struct sCCCSmodel *CCCSmodPtr;  /* backpointer to model */
    struct sCCCSinstance *CCCSnextInstance;  /* pointer to next instance of 
                                             *current model*/
    IFuid CCCSname; /* pointer to character string naming this instance */

    int CCCSposNode;    /* number of positive node of source */
    int CCCSnegNode;    /* number of negative node of source */
    int CCCScontBranch;    /* number of branch eq of controlling source */

    char *CCCScontName; /* pointer to name of controlling instance */

    double CCCScoeff;   /* coefficient */

    double *CCCSposContBrptr;  /* pointer to sparse matrix element at 
                                     *(positive node, control branch eq)*/
    double *CCCSnegContBrptr;  /* pointer to sparse matrix element at 
                                     *(negative node, control branch eq)*/
    unsigned CCCScoeffGiven :1 ;   /* flag to indicate coeff given */

    int  CCCSsenParmNo;   /* parameter # for sensitivity use;
            set equal to  0 if not a design parameter*/

} CCCSinstance ;

/* per model data */

typedef struct sCCCSmodel {       /* model structure for a source */
    int CCCSmodType;    /* type index of this device type */
    struct sCCCSmodel *CCCSnextModel;   /* pointer to next possible model 
                                         *in linked list */
    CCCSinstance * CCCSinstances;       /* pointer to list of instances 
                                             that have this model */
    IFuid CCCSmodName;       /* pointer to character string naming this model */
} CCCSmodel;

/* device parameters */
#define CCCS_GAIN 1
#define CCCS_CONTROL 2
#define CCCS_POS_NODE 3
#define CCCS_NEG_NODE 4
#define CCCS_CONT_BR 5
#define CCCS_GAIN_SENS 6
#define CCCS_CURRENT 7
#define CCCS_POWER 8

/* model parameters */

/* device questions */
#define CCCS_QUEST_SENS_REAL         201
#define CCCS_QUEST_SENS_IMAG         202
#define CCCS_QUEST_SENS_MAG      203
#define CCCS_QUEST_SENS_PH       204
#define CCCS_QUEST_SENS_CPLX         205
#define CCCS_QUEST_SENS_DC       206

/* model questions */

#ifdef __STDC__
extern int CCCSask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int CCCSdelete(GENmodel*,IFuid,GENinstance**);
extern void CCCSdestroy(GENmodel**);
extern int CCCSload(GENmodel*,CKTcircuit*);
extern int CCCSmDelete(GENmodel**,IFuid,GENmodel*);
extern int CCCSparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int CCCSpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int CCCSsAcLoad(GENmodel*,CKTcircuit*);
extern int CCCSsLoad(GENmodel*,CKTcircuit*);
extern void CCCSsPrint(GENmodel*,CKTcircuit*);
extern int CCCSsSetup(SENstruct*,GENmodel*);
extern int CCCSsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
#else /* stdc */
extern int CCCSask();
extern int CCCSdelete();
extern void CCCSdestroy();
extern int CCCSload();
extern int CCCSmDelete();
extern int CCCSparam();
extern int CCCSpzLoad();
extern int CCCSsAcLoad();
extern int CCCSsLoad();
extern void CCCSsPrint();
extern int CCCSsSetup();
extern int CCCSsetup();
#endif /* stdc */

#endif /*CCCS*/
