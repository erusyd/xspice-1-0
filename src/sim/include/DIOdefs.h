/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef DIO
#define DIO "DIOdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:25:37 $ "

#include "IFsim.h"
#include "GENdefs.h"
#include "CKTdefs.h"
#include "complex.h"

            /* data structures used to describe diodes */


/* information needed per instance */

typedef struct sDIOinstance {
    struct sDIOmodel *DIOmodPtr;    /* backpointer to model */
    struct sDIOinstance *DIOnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid DIOname;      /* pointer to character string naming this instance */
    int DIOposNode;     /* number of positive node of diode */
    int DIOnegNode;     /* number of negative node of diode */
    int DIOposPrimeNode;    /* number of positive prime node of diode */
    double *DIOposPosPrimePtr;      /* pointer to sparse matrix at 
                                     * (positive,positive prime) */
    double *DIOnegPosPrimePtr;      /* pointer to sparse matrix at 
                                     * (negative,positive prime) */
    double *DIOposPrimePosPtr;      /* pointer to sparse matrix at 
                                     * (positive prime,positive) */
    double *DIOposPrimeNegPtr;      /* pointer to sparse matrix at 
                                     * (positive prime,negative) */
    double *DIOposPosPtr;   /* pointer to sparse matrix at 
                             * (positive,positive) */
    double *DIOnegNegPtr;   /* pointer to sparse matrix at 
                             * (negative,negative) */
    double *DIOposPrimePosPrimePtr; /* pointer to sparse matrix at 
                                     * (positive prime,positive prime) */

    double DIOcap;   /* stores the diode capacitance */

    double *DIOsens; /* stores the perturbed values of geq and ceq in ac
                         sensitivity analyis */

#define DIOsenGeq DIOsens /* stores the perturbed values of geq */
#define DIOsenCeq DIOsens + 3 /* stores the perturbed values of ceq */
#define DIOdphidp DIOsens + 6 


    int DIOstate;   /* pointer to start of state vector for diode */
#define DIOvoltage DIOstate
#define DIOcurrent DIOstate+1
#define DIOconduct DIOstate+2
#define DIOcapCharge DIOstate+3
#define DIOcapCurrent DIOstate+4
#define DIOsensxp DIOstate+5    /* charge sensitivities and their derivatives.
                                 * +6 for the derivatives - pointer to the
                                 * beginning of the array */

    int DIOsenParmNo ;   /* parameter # for sensitivity use;
                          * set equal to  0 if not a design parameter*/

    unsigned DIOoff : 1;   /* 'off' flag for diode */
    unsigned DIOareaGiven : 1;   /* flag to indicate area was specified */
    unsigned DIOinitCondGiven : 1;  /* flag to indicate ic was specified */
    unsigned DIOsenPertFlag :1; /* indictes whether the the parameter of
                               the particular instance is to be perturbed */
    unsigned DIOtempGiven : 1;  /* flag to indicate temperature was specified */


    double DIOarea;     /* area factor for the diode */
    double DIOinitCond;     /* initial condition */
    double DIOtemp;     /* temperature of the instance */
    double DIOtJctPot;  /* temperature adjusted junction potential */
    double DIOtJctCap;  /* temperature adjusted junction capacitance */
    double DIOtDepCap;  /* temperature adjusted transition point in */
                        /* the curve matching (Fc * Vj ) */
    double DIOtSatCur;  /* temperature adjusted saturation current */
    double DIOtVcrit;   /* temperature adjusted V crit */
    double DIOtF1;      /* temperature adjusted f1 */
    double DIOtBrkdwnV; /* temperature adjusted breakdown voltage */


} DIOinstance ;


/* per model data */

typedef struct sDIOmodel {       /* model structure for a diode */
    int DIOmodType; /* type index of this device type */
    struct sDIOmodel *DIOnextModel; /* pointer to next possible model in 
                                     * linked list */
    DIOinstance * DIOinstances; /* pointer to list of instances 
                                * that have this model */
    IFuid DIOmodName; /* pointer to character string naming this model */

    unsigned DIOsatCurGiven : 1;
    unsigned DIOresistGiven : 1;
    unsigned DIOemissionCoeffGiven : 1;
    unsigned DIOtransitTimeGiven : 1;
    unsigned DIOjunctionCapGiven : 1;
    unsigned DIOjunctionPotGiven : 1;
    unsigned DIOgradingCoeffGiven : 1;
    unsigned DIOactivationEnergyGiven : 1;
    unsigned DIOsaturationCurrentExpGiven : 1;
    unsigned DIOdepletionCapCoeffGiven : 1;
    unsigned DIObreakdownVoltageGiven : 1;
    unsigned DIObreakdownCurrentGiven : 1;
    unsigned DIOnomTempGiven : 1;

    double DIOsatCur;   /* saturation current */
    double DIOresist;   /* ohmic series resistance */
    double DIOconductance;  /* conductance corresponding to ohmic R */
    double DIOemissionCoeff;    /* emission coefficient (N) */
    double DIOtransitTime;      /* transit time (TT) */
    double DIOjunctionCap;      /* Junction Capacitance (Cj0) */
    double DIOjunctionPot;      /* Junction Potential (Vj) or (PB) */
    double DIOgradingCoeff;     /* grading coefficient (m) */
    double DIOactivationEnergy; /* activation energy (EG) */
    double DIOsaturationCurrentExp; /* Saturation current exponential (XTI) */
    double DIOdepletionCapCoeff;    /* Depletion Cap fraction coefficient (FC)*/
    double DIObreakdownVoltage; /* Voltage at reverse breakdown */
    double DIObreakdownCurrent; /* Current at above voltage */
    double DIOf2;   /* coefficient for capacitance equation precomputation */
    double DIOf3;   /* coefficient for capacitance equation precomputation */
    double DIOnomTemp;  /* nominal temperature (temp at which parms measured */

} DIOmodel;

/* device parameters */
#define DIO_AREA 1 
#define DIO_IC 2
#define DIO_OFF 3
#define DIO_CURRENT 4
#define DIO_VOLTAGE 5
#define DIO_CHARGE 6
#define DIO_CAPCUR 7
#define DIO_CONDUCT 8
#define DIO_AREA_SENS 9
#define DIO_POWER 10
#define DIO_TEMP 11
#define DIO_QUEST_SENS_REAL      12
#define DIO_QUEST_SENS_IMAG      13
#define DIO_QUEST_SENS_MAG       14
#define DIO_QUEST_SENS_PH        15
#define DIO_QUEST_SENS_CPLX      16
#define DIO_QUEST_SENS_DC        17

/* model parameters */
#define DIO_MOD_IS 101
#define DIO_MOD_RS 102
#define DIO_MOD_N 103
#define DIO_MOD_TT 104
#define DIO_MOD_CJO 105
#define DIO_MOD_VJ 106
#define DIO_MOD_M 107
#define DIO_MOD_EG 108
#define DIO_MOD_XTI 109
#define DIO_MOD_FC 110
#define DIO_MOD_BV 111
#define DIO_MOD_IBV 112
#define DIO_MOD_D 113
#define DIO_MOD_COND 114
#define DIO_MOD_TNOM 115


#ifdef __STDC__
extern int DIOacLoad(GENmodel*,CKTcircuit*);
extern int DIOask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int DIOconvTest(GENmodel *,CKTcircuit*);
extern int DIOdelete(GENmodel*,IFuid,GENinstance**);
extern void DIOdestroy(GENmodel**);
extern int DIOgetic(GENmodel*,CKTcircuit*);
extern int DIOload(GENmodel*,CKTcircuit*);
extern int DIOmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int DIOmDelete(GENmodel**,IFuid,GENmodel*);
extern int DIOmParam(int,IFvalue*,GENmodel*);
extern int DIOparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int DIOpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int DIOsAcLoad(GENmodel*,CKTcircuit*);
extern int DIOsLoad(GENmodel*,CKTcircuit*);
extern int DIOsSetup(SENstruct*,GENmodel*);
extern void DIOsPrint(GENmodel*,CKTcircuit*);
extern int DIOsUpdate(GENmodel*,CKTcircuit*);
extern int DIOsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int DIOtemp(GENmodel*,CKTcircuit*);
extern int DIOtrunc(GENmodel*,CKTcircuit*,double*);
#else /* stdc */
extern int DIOacLoad();
extern int DIOask();
extern int DIOconvTest();
extern int DIOdelete();
extern void DIOdestroy();
extern int DIOgetic();
extern int DIOload();
extern int DIOmAsk();
extern int DIOmDelete();
extern int DIOmParam();
extern int DIOparam();
extern int DIOpzLoad();
extern int DIOsAcLoad();
extern int DIOsLoad();
extern int DIOsSetup();
extern void DIOsPrint();
extern int DIOsUpdate();
extern int DIOsetup();
extern int DIOtemp();
extern int DIOtrunc();
#endif /* stdc */

#endif /*DIO*/
