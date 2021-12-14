/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef CAP
#define CAP "CAPdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:25:14 $ "

#include "IFsim.h"
#include "complex.h"
#include "GENdefs.h"
#include "CKTdefs.h"

    /* structures used to describe capacitors */


/* information to describe each instance */

typedef struct sCAPinstance {
    struct sCAPmodel *CAPmodPtr;    /* backpointer to model */
    struct sCAPinstance *CAPnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid CAPname;  /* pointer to character string naming this instance */
    int CAPposNode; /* number of positive node of capacitor */
    int CAPnegNode; /* number of negative node of capacitor */
    double CAPcapac;    /* capacitance */
    double CAPinitCond; /* initial capacitor voltage if specified */
    double CAPwidth;    /* width of the capacitor */
    double CAPlength;   /* length of the capacitor */
    int CAPstate;   /* pointer to start of capacitor state vector */
#define CAPqcap CAPstate    /* charge on the capacitor */
#define CAPccap CAPstate+1  /* current through the capacitor */
#define CAPsensxp CAPstate+2 /* charge sensitivities and their derivatives.
                                +3 for the derivatives - pointer to the
                beginning of the array */
                            

    double *CAPposPosptr;    /* pointer to sparse matrix diagonal at 
                             * (positive,positive) */
    double *CAPnegNegptr;    /* pointer to sparse matrix diagonal at 
                             * (negative,negative) */
    double *CAPposNegptr;    /* pointer to sparse matrix offdiagonal at 
                             * (positive,negative) */
    double *CAPnegPosptr;    /* pointer to sparse matrix offdiagonal at 
                             * (negative,positive) */
    unsigned CAPcapGiven : 1;   /* flag to indicate capacitance was specified */
    unsigned CAPicGiven  : 1;   /* flag to indicate init. cond. was specified */
    unsigned CAPwidthGiven  : 1;    /* flag to indicate capacitor width given */
    unsigned CAPlengthGiven : 1;    /* flag to indicate capacitor length given*/
    int    CAPsenParmNo;         /* parameter # for sensitivity use;
                set equal to  0 if not a design parameter*/

} CAPinstance ;

/* data per model */

typedef struct sCAPmodel {      /* model structure for a capacitor */
    int CAPmodType; /* type index of this device type */
    struct sCAPmodel *CAPnextModel; /* pointer to next possible model in 
                                     * linked list */
    CAPinstance * CAPinstances; /* pointer to list of instances that have this
                                 * model */
    IFuid CAPmodName;   /* pointer to character string naming this model */
    double CAPcj;       /* Unit Area Capacitance ( F/ M**2 ) */
    double CAPcjsw;     /* Unit Length Sidewall Capacitance ( F / M ) */
    double CAPdefWidth; /* the default width of a capacitor */
    double CAPnarrow;   /* amount by which length/width are less than drawn */
    unsigned CAPcjGiven : 1;    /* Unit Area Capacitance ( F/ M**2 ) */
    unsigned CAPcjswGiven   : 1;    /* Unit Length Sidewall Capacitance( F/M )*/
    unsigned CAPdefWidthGiven   : 1;    /* flag indicates default width given*/
    unsigned CAPnarrowGiven : 1;    /* flag indicates narrowing factor given */
} CAPmodel;

/* device parameters */
#define CAP_CAP 1
#define CAP_IC 2
#define CAP_WIDTH 3
#define CAP_LENGTH 4
#define CAP_CAP_SENS 5
#define CAP_CURRENT 6
#define CAP_POWER 7

/* model parameters */
#define CAP_MOD_CJ 101
#define CAP_MOD_CJSW 102
#define CAP_MOD_DEFWIDTH 103
#define CAP_MOD_C 104
#define CAP_MOD_NARROW 105

/* device questions */
#define CAP_QUEST_SENS_REAL      201
#define CAP_QUEST_SENS_IMAG      202
#define CAP_QUEST_SENS_MAG       203
#define CAP_QUEST_SENS_PH        204
#define CAP_QUEST_SENS_CPLX      205
#define CAP_QUEST_SENS_DC        206

#ifdef __STDC__
extern int CAPacLoad(GENmodel*,CKTcircuit*);
extern int CAPask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int CAPdelete(GENmodel*,IFuid,GENinstance**);
extern void CAPdestroy(GENmodel**);
extern int CAPgetic(GENmodel*,CKTcircuit*);
extern int CAPload(GENmodel*,CKTcircuit*);
extern int CAPmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int CAPmDelete(GENmodel**,IFuid,GENmodel*);
extern int CAPmParam(int,IFvalue*,GENmodel*);
extern int CAPparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int CAPpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int CAPsAcLoad(GENmodel*,CKTcircuit*);
extern int CAPsLoad(GENmodel*,CKTcircuit*);
extern void CAPsPrint(GENmodel*,CKTcircuit*);
extern int CAPsSetup(SENstruct *,GENmodel*);
extern int CAPsUpdate(GENmodel*,CKTcircuit*);
extern int CAPsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int CAPtemp(GENmodel*,CKTcircuit*);
extern int CAPtrunc(GENmodel*,CKTcircuit*,double*);
#else /* stdc */
extern int CAPacLoad();
extern int CAPask();
extern int CAPdelete();
extern void CAPdestroy();
extern int CAPgetic();
extern int CAPload();
extern int CAPmAsk();
extern int CAPmDelete();
extern int CAPmParam();
extern int CAPparam();
extern int CAPpzLoad();
extern int CAPsAcLoad();
extern int CAPsLoad();
extern void CAPsPrint();
extern int CAPsSetup();
extern int CAPsUpdate();
extern int CAPsetup();
extern int CAPtemp();
extern int CAPtrunc();
#endif /* stdc */


#endif /*CAP*/
