/*
 * Copyright (c) 1985 S. Hwang
 */
#ifndef MES
#define MES "MESdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:16 $ "

#include "IFsim.h"
#include "CKTdefs.h"
#include "GENdefs.h"
#include "complex.h"

    /* structures used to describe MESFET Transistors */


/* information used to describe a single instance */

typedef struct sMESinstance {
    struct sMESmodel *MESmodPtr;    /* backpointer to model */
    struct sMESinstance *MESnextInstance; /* pointer to next instance of 
                                             * current model*/
    IFuid MESname; /* pointer to character string naming this instance */

    int MESdrainNode;  /* number of drain node of mesfet */
    int MESgateNode;   /* number of gate node of mesfet */
    int MESsourceNode; /* number of source node of mesfet */
    int MESdrainPrimeNode; /* number of internal drain node of mesfet */
    int MESsourcePrimeNode;    /* number of internal source node of mesfet */
    double MESarea;    /* area factor for the mesfet */
    double MESicVDS;   /* initial condition voltage D-S*/
    double MESicVGS;   /* initial condition voltage G-S*/
    double *MESdrainDrainPrimePtr; /* pointer to sparse matrix at 
                                     * (drain,drain prime) */
    double *MESgateDrainPrimePtr;  /* pointer to sparse matrix at 
                                     * (gate,drain prime) */
    double *MESgateSourcePrimePtr; /* pointer to sparse matrix at 
                                     * (gate,source prime) */
    double *MESsourceSourcePrimePtr;   /* pointer to sparse matrix at 
                                         * (source,source prime) */
    double *MESdrainPrimeDrainPtr; /* pointer to sparse matrix at 
                                     * (drain prime,drain) */
    double *MESdrainPrimeGatePtr;  /* pointer to sparse matrix at 
                                     * (drain prime,gate) */
    double *MESdrainPrimeSourcePrimePtr;   /* pointer to sparse matrix
                                             * (drain prime,source prime) */
    double *MESsourcePrimeGatePtr; /* pointer to sparse matrix at 
                                     * (source prime,gate) */
    double *MESsourcePrimeSourcePtr;   /* pointer to sparse matrix at 
                                         * (source prime,source) */
    double *MESsourcePrimeDrainPrimePtr;   /* pointer to sparse matrix
                                             * (source prime,drain prime) */
    double *MESdrainDrainPtr;  /* pointer to sparse matrix at 
                                 * (drain,drain) */
    double *MESgateGatePtr;    /* pointer to sparse matrix at 
                                 * (gate,gate) */
    double *MESsourceSourcePtr;    /* pointer to sparse matrix at 
                                     * (source,source) */
    double *MESdrainPrimeDrainPrimePtr;    /* pointer to sparse matrix
                                             * (drain prime,drain prime) */
    double *MESsourcePrimeSourcePrimePtr;  /* pointer to sparse matrix
                                             * (source prime,source prime) */

    int MESstate; /* pointer to start of state vector for mesfet */
#define MESvgs MESstate 
#define MESvgd MESstate+1 
#define MEScg MESstate+2 
#define MEScd MESstate+3 
#define MEScgd MESstate+4 
#define MESgm MESstate+5 
#define MESgds MESstate+6 
#define MESggs MESstate+7 
#define MESggd MESstate+8 
#define MESqgs MESstate+9 
#define MEScqgs MESstate+10 
#define MESqgd MESstate+11 
#define MEScqgd MESstate+12 
    int MESoff;   /* 'off' flag for mesfet */
    unsigned MESareaGiven : 1;   /* flag to indicate area was specified */
    unsigned MESicVDSGiven : 1;   /* initial condition given flag for V D-S*/
    unsigned MESicVGSGiven : 1;   /* initial condition given flag for V G-S*/
} MESinstance ;


/* per model data */

typedef struct sMESmodel {       /* model structure for a mesfet */
    int MESmodType; /* type index of this device type */
    struct sMESmodel *MESnextModel;   /* pointer to next possible model in 
                                         * linked list */
    MESinstance * MESinstances; /* pointer to list of instances 
                                   * that have this model */
    IFuid MESmodName; /* pointer to character string naming this model */
    int MEStype;

    double MESthreshold;
    double MESalpha;
    double MESbeta;
    double MESlModulation;
    double MESb;
    double MESdrainResist;
    double MESsourceResist;
    double MEScapGS;
    double MEScapGD;
    double MESgatePotential;
    double MESgateSatCurrent;
    double MESdepletionCapCoeff;

    double MESdrainConduct;
    double MESsourceConduct;
    double MESdepletionCap;
    double MESf1;
    double MESf2;
    double MESf3;
    double MESvcrit;

    unsigned MESthresholdGiven : 1;
    unsigned MESalphaGiven : 1;
    unsigned MESbetaGiven : 1;
    unsigned MESlModulationGiven : 1;
    unsigned MESbGiven : 1;
    unsigned MESdrainResistGiven : 1;
    unsigned MESsourceResistGiven : 1;
    unsigned MEScapGSGiven : 1;
    unsigned MEScapGDGiven : 1;
    unsigned MESgatePotentialGiven : 1;
    unsigned MESgateSatCurrentGiven : 1;
    unsigned MESdepletionCapCoeffGiven : 1;

} MESmodel;

#ifndef NMF

#define NMF 1
#define PMF -1

#endif /*NMF*/

/* device parameters */
#define MES_AREA 1
#define MES_IC_VDS 2
#define MES_IC_VGS 3
#define MES_IC 4
#define MES_OFF 5
#define MES_CS 6
#define MES_POWER 7

/* model parameters */
#define MES_MOD_VTO 101
#define MES_MOD_ALPHA 102
#define MES_MOD_BETA 103
#define MES_MOD_LAMBDA 104
#define MES_MOD_B 105
#define MES_MOD_RD 106
#define MES_MOD_RS 107
#define MES_MOD_CGS 108
#define MES_MOD_CGD 109
#define MES_MOD_PB 110
#define MES_MOD_IS 111
#define MES_MOD_FC 112
#define MES_MOD_NMF 113
#define MES_MOD_PMF 114

/* device questions */

#define MES_DRAINNODE       201
#define MES_GATENODE        202
#define MES_SOURCENODE      203
#define MES_DRAINPRIMENODE  204
#define MES_SOURCEPRIMENODE 205

#define MES_VGS         206
#define MES_VGD         207
#define MES_CG          208
#define MES_CD          209
#define MES_CGD         210
#define MES_GM          211
#define MES_GDS         212
#define MES_GGS         213
#define MES_GGD         214
#define MES_QGS         215
#define MES_CQGS        216
#define MES_QGD         217
#define MES_CQGD        218

/* model questions */

#define MES_MOD_DRAINCONDUCT    301
#define MES_MOD_SOURCECONDUCT   302 
#define MES_MOD_DEPLETIONCAP    303
#define MES_MOD_VCRIT       304

#ifdef __STDC__
extern int MESacLoad(GENmodel*,CKTcircuit*);
extern int MESask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int MESdelete(GENmodel*,IFuid,GENinstance**);
extern void MESdestroy(GENmodel**);
extern int MESgetic(GENmodel*,CKTcircuit*);
extern int MESload(GENmodel*,CKTcircuit*);
extern int MESmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int MESmDelete(GENmodel**,IFuid,GENmodel*);
extern int MESmParam(int,IFvalue*,GENmodel*);
extern int MESparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int MESpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int MESsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int MEStemp(GENmodel*,CKTcircuit*);
extern int MEStrunc(GENmodel*,CKTcircuit*,double*);
#else /* stdc */
extern int MESacLoad();
extern int MESask();
extern int MESdelete();
extern void MESdestroy();
extern int MESgetic();
extern int MESload();
extern int MESmAsk();
extern int MESmDelete();
extern int MESmParam();
extern int MESparam();
extern int MESpzLoad();
extern int MESsetup();
extern int MEStemp();
extern int MEStrunc();
#endif /* stdc */

#endif /*MES*/
