/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef JFET
#define JFET "JFETdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:13 $ "

#include "IFsim.h"
#include "GENdefs.h"
#include "CKTdefs.h"
#include "complex.h"

    /* structures used to describe Junction Field Effect Transistors */


/* information used to describe a single instance */

typedef struct sJFETinstance {
    struct sJFETmodel *JFETmodPtr;  /* backpointer to model */
    struct sJFETinstance *JFETnextInstance; /* pointer to next instance of 
                                             * current model*/
    IFuid JFETname; /* pointer to character string naming this instance */
    int JFETdrainNode;  /* number of drain node of jfet */
    int JFETgateNode;   /* number of gate node of jfet */
    int JFETsourceNode; /* number of source node of jfet */
    int JFETdrainPrimeNode; /* number of internal drain node of jfet */
    int JFETsourcePrimeNode;    /* number of internal source node of jfet */
    double *JFETdrainDrainPrimePtr; /* pointer to sparse matrix at 
                                     * (drain,drain prime) */
    double *JFETgateDrainPrimePtr;  /* pointer to sparse matrix at 
                                     * (gate,drain prime) */
    double *JFETgateSourcePrimePtr; /* pointer to sparse matrix at 
                                     * (gate,source prime) */
    double *JFETsourceSourcePrimePtr;   /* pointer to sparse matrix at 
                                         * (source,source prime) */
    double *JFETdrainPrimeDrainPtr; /* pointer to sparse matrix at 
                                     * (drain prime,drain) */
    double *JFETdrainPrimeGatePtr;  /* pointer to sparse matrix at 
                                     * (drain prime,gate) */
    double *JFETdrainPrimeSourcePrimePtr;   /* pointer to sparse matrix
                                             * (drain prime,source prime) */
    double *JFETsourcePrimeGatePtr; /* pointer to sparse matrix at 
                                     * (source prime,gate) */
    double *JFETsourcePrimeSourcePtr;   /* pointer to sparse matrix at 
                                         * (source prime,source) */
    double *JFETsourcePrimeDrainPrimePtr;   /* pointer to sparse matrix
                                             * (source prime,drain prime) */
    double *JFETdrainDrainPtr;  /* pointer to sparse matrix at 
                                 * (drain,drain) */
    double *JFETgateGatePtr;    /* pointer to sparse matrix at 
                                 * (gate,gate) */
    double *JFETsourceSourcePtr;    /* pointer to sparse matrix at 
                                     * (source,source) */
    double *JFETdrainPrimeDrainPrimePtr;    /* pointer to sparse matrix
                                             * (drain prime,drain prime) */
    double *JFETsourcePrimeSourcePrimePtr;  /* pointer to sparse matrix
                                             * (source prime,source prime) */

    int JFETstate; /* pointer to start of state vector for jfet */
#define JFETvgs JFETstate 
#define JFETvgd JFETstate+1 
#define JFETcg JFETstate+2 
#define JFETcd JFETstate+3 
#define JFETcgd JFETstate+4 
#define JFETgm JFETstate+5 
#define JFETgds JFETstate+6 
#define JFETggs JFETstate+7 
#define JFETggd JFETstate+8 
#define JFETqgs JFETstate+9 
#define JFETcqgs JFETstate+10 
#define JFETqgd JFETstate+11 
#define JFETcqgd JFETstate+12 
    unsigned JFEToff :1;            /* 'off' flag for jfet */
    unsigned JFETareaGiven  : 1;    /* flag to indicate area was specified */
    unsigned JFETicVDSGiven : 1;    /* initial condition given flag for V D-S*/
    unsigned JFETicVGSGiven : 1;    /* initial condition given flag for V G-S*/
    unsigned JFETtempGiven  : 1;    /* flag to indicate instance temp given */


    double JFETarea;    /* area factor for the jfet */
    double JFETicVDS;   /* initial condition voltage D-S*/
    double JFETicVGS;   /* initial condition voltage G-S*/
    double JFETtemp;    /* operating temperature */
    double JFETtSatCur; /* temperature adjusted saturation current */
    double JFETtGatePot;    /* temperature adjusted gate potential */
    double JFETtCGS;    /* temperature corrected G-S capacitance */
    double JFETtCGD;    /* temperature corrected G-D capacitance */
    double JFETcorDepCap;   /* joining point of the fwd bias dep. cap eq.s */
    double JFETvcrit;   /* critical voltage for the instance */
    double JFETf1;      /* coefficient of capacitance polynomial exp */


} JFETinstance ;


/* per model data */

typedef struct sJFETmodel {       /* model structure for a jfet */
    int JFETmodType;    /* type index of this device type */
    struct sJFETmodel *JFETnextModel;   /* pointer to next possible model in 
                                         * linked list */
    JFETinstance * JFETinstances; /* pointer to list of instances 
                                   * that have this model */
    IFuid JFETmodName; /* pointer to character string naming this model */
    int JFETtype;

    double JFETthreshold;
    double JFETbeta;
    double JFETlModulation;
    double JFETdrainResist;
    double JFETsourceResist;
    double JFETcapGS;
    double JFETcapGD;
    double JFETgatePotential;
    double JFETgateSatCurrent;
    double JFETdepletionCapCoeff;

    double JFETdrainConduct;
    double JFETsourceConduct;
    double JFETf2;
    double JFETf3;
    double JFETtnom;    /* temperature at which parameters were measured */

    unsigned JFETthresholdGiven : 1;
    unsigned JFETbetaGiven : 1;
    unsigned JFETlModulationGiven : 1;
    unsigned JFETdrainResistGiven : 1;
    unsigned JFETsourceResistGiven : 1;
    unsigned JFETcapGSGiven : 1;
    unsigned JFETcapGDGiven : 1;
    unsigned JFETgatePotentialGiven : 1;
    unsigned JFETgateSatCurrentGiven : 1;
    unsigned JFETdepletionCapCoeffGiven : 1;
    unsigned JFETtnomGiven : 1; /* user specified Tnom for model */

} JFETmodel;

#ifndef NJF

#define NJF 1
#define PJF -1

#endif /*NJF*/

/* device parameters */
#define JFET_AREA 1
#define JFET_IC_VDS 2
#define JFET_IC_VGS 3
#define JFET_IC 4
#define JFET_OFF 5
#define JFET_TEMP 6

/* model parameters */
#define JFET_MOD_VTO 101
#define JFET_MOD_BETA 102
#define JFET_MOD_LAMBDA 103
#define JFET_MOD_RD 104
#define JFET_MOD_RS 105
#define JFET_MOD_CGS 106
#define JFET_MOD_CGD 107
#define JFET_MOD_PB 108
#define JFET_MOD_IS 109
#define JFET_MOD_FC 110 
#define JFET_MOD_NJF 111
#define JFET_MOD_PJF 112
#define JFET_MOD_TNOM 113

/* device questions */
#define JFET_DRAINNODE        301
#define JFET_GATENODE         302
#define JFET_SOURCENODE       303
#define JFET_DRAINPRIMENODE   304
#define JFET_SOURCEPRIMENODE  305
#define JFET_VGS              306
#define JFET_VGD              307
#define JFET_CG               308
#define JFET_CD               309
#define JFET_CGD              310
#define JFET_GM               311
#define JFET_GDS              312
#define JFET_GGS              313
#define JFET_GGD              314
#define JFET_QGS              315
#define JFET_CQGS             316
#define JFET_QGD              317
#define JFET_CQGD             318
#define JFET_CS               319
#define JFET_POWER            320

/* model questions */
#define JFET_MOD_DRAINCONDUCT   301
#define JFET_MOD_SOURCECONDUCT  302
#define JFET_MOD_DEPLETIONCAP   303
#define JFET_MOD_VCRIT          304

/* function definitions */

#ifdef __STDC__
extern int JFETacLoad(GENmodel*,CKTcircuit*);
extern int JFETask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int JFETdelete(GENmodel*,IFuid,GENinstance**);
extern void JFETdestroy(GENmodel**);
extern int JFETgetic(GENmodel*,CKTcircuit*);
extern int JFETload(GENmodel*,CKTcircuit*);
extern int JFETmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int JFETmDelete(GENmodel**,IFuid,GENmodel*);
extern int JFETmParam(int,IFvalue*,GENmodel*);
extern int JFETparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int JFETpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int JFETsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int JFETtemp(GENmodel*,CKTcircuit*);
extern int JFETtrunc(GENmodel*,CKTcircuit*,double*);
#else /* stdc */
extern int JFETacLoad();
extern int JFETask();
extern int JFETdelete();
extern void JFETdestroy();
extern int JFETgetic();
extern int JFETload();
extern int JFETmAsk();
extern int JFETmDelete();
extern int JFETmParam();
extern int JFETparam();
extern int JFETpzLoad();
extern int JFETsetup();
extern int JFETtemp();
extern int JFETtrunc();
#endif /* stdc */

#endif /*JFET*/
