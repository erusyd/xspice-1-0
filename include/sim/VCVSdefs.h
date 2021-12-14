/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef VCVS
#define VCVS "VCVSdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:51 $ "

#include "IFsim.h"
#include "CKTdefs.h"
#include "GENdefs.h"
#include "complex.h"

        /*
         * structures to describe Voltage Controlled Voltage Sources
         */

/* information to describe a single instance */

typedef struct sVCVSinstance {
    struct sVCVSmodel *VCVSmodPtr;  /* backpointer to model */
    struct sVCVSinstance *VCVSnextInstance;  /* pointer to next instance of 
                                              *current model*/
    IFuid VCVSname; /* pointer to character string naming this instance */

    int VCVSposNode;    /* number of positive node of source */
    int VCVSnegNode;    /* number of negative node of source */
    int VCVScontPosNode;    /* number of positive node of controlling source */
    int VCVScontNegNode;    /* number of negative node of controlling source */
    int VCVSbranch; /* equation number of branch equation added for v source */

    double VCVSinitCond;    /* initial condition (of controlling source) */
    double VCVScoeff;   /* coefficient */
    int VCVSstates; /* state info */
#define VCVSvOld VCVSstates
#define VCVScontVOld VCVSstates + 1

    double *VCVSposIbrptr;  /* pointer to sparse matrix element at 
                             * (positive node, branch equation) */
    double *VCVSnegIbrptr;  /* pointer to sparse matrix element at 
                             * (negative node, branch equation) */
    double *VCVSibrPosptr;  /* pointer to sparse matrix element at 
                             * (branch equation, positive node) */
    double *VCVSibrNegptr;  /* pointer to sparse matrix element at 
                             * (branch equation, negative node) */
    double *VCVSibrContPosptr;  /* pointer to sparse matrix element at 
                                 *(branch equation, control positive node)*/
    double *VCVSibrContNegptr;  /* pointer to sparse matrix element at 
                                 *(branch equation, control negative node)*/
    unsigned VCVScoeffGiven :1 ;/* flag to indicate function coeffs given */

    int  VCVSsenParmNo;   /* parameter # for sensitivity use;
            set equal to  0 if not a design parameter*/

} VCVSinstance ;


/* per model data */

typedef struct sVCVSmodel {       /* model structure for a source */
    int VCVSmodType;    /* type index of this device type */
    struct sVCVSmodel *VCVSnextModel;   /* pointer to next possible model 
                                         *in linked list */
    VCVSinstance * VCVSinstances;    /* pointer to list of instances 
                                      * that have this model */
    IFuid VCVSmodName;       /* pointer to character string naming this model */
} VCVSmodel;

/* device parameters */
#define VCVS_GAIN 1
#define VCVS_POS_NODE 2
#define VCVS_NEG_NODE 3
#define VCVS_CONT_P_NODE 4
#define VCVS_CONT_N_NODE 5
#define VCVS_BR 6
#define VCVS_IC 7
#define VCVS_CONT_V_OLD 8
#define VCVS_GAIN_SENS 9
#define VCVS_CURRENT 10
#define VCVS_POWER 11

/* model parameters */

/* device questions */
#define VCVS_QUEST_SENS_REAL         201
#define VCVS_QUEST_SENS_IMAG         202
#define VCVS_QUEST_SENS_MAG      203
#define VCVS_QUEST_SENS_PH       204
#define VCVS_QUEST_SENS_CPLX         205
#define VCVS_QUEST_SENS_DC       206

/* model questions */

#ifdef __STDC__
extern int VCVSask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int VCVSdelete(GENmodel*,IFuid,GENinstance**);
extern void VCVSdestroy(GENmodel**);
extern int VCVSfindBr(CKTcircuit*,GENmodel*,IFuid);
extern int VCVSload(GENmodel*,CKTcircuit*);
extern int VCVSmDelete(GENmodel**,IFuid,GENmodel*);
extern int VCVSparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int VCVSpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int VCVSsAcLoad(GENmodel*,CKTcircuit*);
extern int VCVSsLoad(GENmodel*,CKTcircuit*);
extern int VCVSsSetup(SENstruct*,GENmodel*);
extern void VCVSsPrint(GENmodel*,CKTcircuit*);
extern int VCVSsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
#else /* stdc */
extern int VCVSask();
extern int VCVSdelete();
extern void VCVSdestroy();
extern int VCVSfindBr();
extern int VCVSload();
extern int VCVSmDelete();
extern int VCVSparam();
extern int VCVSpzLoad();
extern int VCVSsAcLoad();
extern int VCVSsLoad();
extern int VCVSsSetup();
extern void VCVSsPrint();
extern int VCVSsetup();
#endif /* stdc */

#endif /*VCVS*/
