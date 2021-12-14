/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef VSRC
#define VSRC "VSRCdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:52 $ "

#include "IFsim.h"
#include "CKTdefs.h"
#include "GENdefs.h"
#include "complex.h"

        /*
         * structures to describe independent voltage sources
         */


/* information needed for each instance */

typedef struct sVSRCinstance {
    struct sVSRCmodel *VSRCmodPtr;  /* backpointer to model */
    struct sVSRCinstance *VSRCnextInstance;  /* pointer to next instance of 
                                              *current model */
    IFuid VSRCname; /* pointer to character string naming this instance */

    int VSRCposNode;    /* number of positive node of resistor */
    int VSRCnegNode;    /* number of negative node of resistor */

    int VSRCbranch; /* equation number of branch equation added for source */

    int VSRCfunctionType;   /* code number of function type for source */
    int VSRCfunctionOrder;  /* order of the function for the source */
    double *VSRCcoeffs; /* pointer to array of coefficients */

    double VSRCdcValue; /* DC and TRANSIENT value of source */

    double VSRCacPhase; /* AC phase angle */
    double VSRCacMag; /* AC magnitude */

    double VSRCacReal; /* AC real component */
    double VSRCacImag; /* AC imaginary component */

    double *VSRCposIbrptr;  /* pointer to sparse matrix element at 
                             * (positive node, branch equation) */
    double *VSRCnegIbrptr;  /* pointer to sparse matrix element at 
                             * (negative node, branch equation) */
    double *VSRCibrPosptr;  /* pointer to sparse matrix element at 
                             * (branch equation, positive node) */
    double *VSRCibrNegptr;  /* pointer to sparse matrix element at 
                             * (branch equation, negative node) */
    double *VSRCibrIbrptr;  /* pointer to sparse matrix element at 
                             * (branch equation, branch equation) */
    unsigned VSRCdcGiven     :1 ;   /* flag to indicate dc value given */
    unsigned VSRCacGiven     :1 ;   /* flag to indicate ac keyword given */
    unsigned VSRCacMGiven    :1 ;   /* flag to indicate ac magnitude given */
    unsigned VSRCacPGiven    :1 ;   /* flag to indicate ac phase given */
    unsigned VSRCfuncTGiven  :1 ;   /* flag to indicate function type given */
    unsigned VSRCcoeffsGiven :1 ;   /* flag to indicate function coeffs given */
} VSRCinstance ;


/* per model data */

typedef struct sVSRCmodel {
    int VSRCmodType;    /* type index of this device type */
    struct sVSRCmodel *VSRCnextModel;    /* pointer to next possible model 
                                          *in linked list */
    VSRCinstance * VSRCinstances;    /* pointer to list of instances 
                                      * that have this model */
    IFuid VSRCmodName;       /* pointer to character string naming this model */
} VSRCmodel;

/* source function types (shared with current sources) */
#ifndef PULSE
#define PULSE 1
#define SINE 2
#define EXP 3
#define SFFM 4
#define PWL 5
#endif /*PULSE*/

/* device parameters */
#define VSRC_DC 1
#define VSRC_AC 2
#define VSRC_AC_MAG 3
#define VSRC_AC_PHASE 4
#define VSRC_PULSE 5
#define VSRC_SINE 6
#define VSRC_EXP 7
#define VSRC_PWL 8
#define VSRC_SFFM 9
#define VSRC_BR 10
#define VSRC_FCN_TYPE 11
#define VSRC_FCN_ORDER 12
#define VSRC_FCN_COEFFS 13
#define VSRC_AC_REAL 14
#define VSRC_AC_IMAG 15
#define VSRC_POS_NODE 16
#define VSRC_NEG_NODE 17
#define VSRC_CURRENT 18
#define VSRC_POWER 19

/* model parameters */

/* device questions */

/* model questions */

#ifdef __STDC__
extern int VSRCacLoad(GENmodel*,CKTcircuit*);
extern int VSRCaccept(CKTcircuit*,GENmodel *);
extern int VSRCask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int VSRCdelete(GENmodel*,IFuid,GENinstance**);
extern void VSRCdestroy(GENmodel**);
extern int VSRCfindBr(CKTcircuit*,GENmodel*,IFuid);
extern int VSRCload(GENmodel*,CKTcircuit*);
extern int VSRCmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int VSRCmDelete(GENmodel**,IFuid,GENmodel*);
extern int VSRCparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int VSRCpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int VSRCsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int VSRCpzSetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int VSRCtemp(GENmodel*,CKTcircuit*);
#else /* stdc */
extern int VSRCacLoad();
extern int VSRCaccept();
extern int VSRCask();
extern int VSRCdelete();
extern void VSRCdestroy();
extern int VSRCfindBr();
extern int VSRCload();
extern int VSRCmAsk();
extern int VSRCmDelete();
extern int VSRCparam();
extern int VSRCpzLoad();
extern int VSRCsetup();
extern int VSRCpzSetup();
extern int VSRCtemp();
#endif /* stdc */

#endif /*VSRC*/
