/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef ISRC
#define ISRC "ISRCdefs.h $Revision: 1.2 $  on $Date: 91/11/08 16:09:22 $ "

#include "IFsim.h"
#include "complex.h"
#include "CKTdefs.h"
#include "GENdefs.h"

    /* structures used to describe current sources */


/* information needed per source instance */

typedef struct sISRCinstance {
    struct sISRCmodel *ISRCmodPtr;  /* backpointer to model */
    struct sISRCinstance *ISRCnextInstance;  /* pointer to next instance of 
                                              *current model*/
    IFuid ISRCname; /* pointer to character string naming this instance */

    int ISRCnegNode;    /* number of negative node of source */
    int ISRCposNode;    /* number of positive node of source */

    int ISRCfunctionType;   /* code number of function type for source */
    int ISRCfunctionOrder;  /* order of the function for the source */
    double *ISRCcoeffs; /* pointer to array of coefficients */

    double ISRCdcValue; /* DC and TRANSIENT value of source */
    double ISRCacPhase; /* AC phase angle */
    double ISRCacMag; /* AC magnitude */
    double ISRCacReal; /* AC real part */
    double ISRCacImag; /* AC imaginary */


    unsigned ISRCdcGiven     :1 ;   /* flag to indicate dc value given */
    unsigned ISRCacGiven     :1 ;   /* flag to indicate ac keyword given */
    unsigned ISRCacMGiven    :1 ;   /* flag to indicate ac magnitude given */
    unsigned ISRCacPGiven    :1 ;   /* flag to indicate ac phase given */
    unsigned ISRCfuncTGiven  :1 ;   /* flag to indicate function type given */
    unsigned ISRCcoeffsGiven :1 ;   /* flag to indicate function coeffs given */

    /* gtri - begin - add member to hold current source value */
    /* needed for outputting results */
    double ISRCcurrent; /* current value */
    /* gtri - end - add member to hold current source value */

} ISRCinstance ;


/* per model data */

typedef struct sISRCmodel {       /* model structure for a resistor */
    int ISRCmodType;    /* type index of this device type */
    struct sISRCmodel *ISRCnextModel;    /* pointer to next possible model 
                                          *in linked list */
    ISRCinstance * ISRCinstances;    /* pointer to list of instances 
                                             * that have this model */
    IFuid ISRCmodName;       /* pointer to character string naming this model */
} ISRCmodel;


/* source types */

#ifndef PULSE
#define PULSE 1
#define SINE 2
#define EXP 3
#define SFFM 4
#define PWL 5
#endif /*PULSE*/

/* device parameters */
#define ISRC_DC 1
#define ISRC_AC_MAG 2
#define ISRC_AC_PHASE 3
#define ISRC_AC 4
#define ISRC_PULSE 5
#define ISRC_SINE 6
#define ISRC_EXP 7
#define ISRC_PWL 8
#define ISRC_SFFM 9
#define ISRC_NEG_NODE 10
#define ISRC_POS_NODE 11
#define ISRC_AC_REAL 12
#define ISRC_AC_IMAG 13
#define ISRC_FCN_TYPE 14
#define ISRC_FCN_ORDER 15
#define ISRC_FCN_COEFFS 16
#define ISRC_POWER 17

/* gtri - begin - add define for current source value */
/* needed for outputting results */
#define ISRC_CURRENT 18
/* gtri - end - add define for current source value */

/* model parameters */

/* device questions */

/* model questions */


#ifdef __STDC__
extern int ISRCaccept(CKTcircuit*,GENmodel*);
extern int ISRCacLoad(GENmodel*,CKTcircuit*);
extern int ISRCask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int ISRCdelete(GENmodel*,IFuid,GENinstance**);
extern void ISRCdestroy(GENmodel**);
extern int ISRCload(GENmodel*,CKTcircuit*);
extern int ISRCmDelete(GENmodel**,IFuid,GENmodel*);
extern int ISRCparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int ISRCpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int ISRCtemp(GENmodel*,CKTcircuit*);
#else /* stdc */
extern int ISRCaccept();
extern int ISRCacLoad();
extern int ISRCask();
extern int ISRCdelete();
extern void ISRCdestroy();
extern int ISRCload();
extern int ISRCmDelete();
extern int ISRCparam();
extern int ISRCpzLoad();
extern int ISRCtemp();
#endif /* stdc */

#endif /*ISRC*/
