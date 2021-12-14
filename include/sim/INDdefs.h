/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef IND
#define IND "INDdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:05 $ "

/* turn on mutual inductor code */
#define MUTUAL

#include "IFsim.h"
#include "complex.h"
#include "GENdefs.h"
#include "CKTdefs.h"

        /* structures used to descrive inductors */


/* information needed for each instance */

typedef struct sINDinstance {
    struct sINDmodel *INDmodPtr;    /* backpointer to model */
    struct sINDinstance *INDnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid INDname;  /* pointer to character string naming this instance */
    int INDposNode; /* number of positive node of inductor */
    int INDnegNode; /* number of negative node of inductor */
    int INDbrEq;    /* number of the branch equation added for current */
    int INDstate;   /* pointer to beginning of state vector for inductor */
    double INDinduct;    /* inductance */
    double INDinitCond; /* initial inductor voltage if specified */
#define INDflux INDstate    /* flux in the inductor */
#define INDvolt INDstate+1  /* voltage - save an entry in table */
#define INDsensxp INDstate+2 /* charge sensitivities and their derivatives.
                                +3 for the derivatives - pointer to the
                beginning of the array */

    double *INDposIbrptr;    /* pointer to sparse matrix diagonal at 
                              * (positive,branch eq) */
    double *INDnegIbrptr;    /* pointer to sparse matrix diagonal at 
                              * (negative,branch eq) */
    double *INDibrNegptr;    /* pointer to sparse matrix offdiagonal at 
                              * (branch eq,negative) */
    double *INDibrPosptr;    /* pointer to sparse matrix offdiagonal at 
                              * (branch eq,positive) */
    double *INDibrIbrptr;    /* pointer to sparse matrix offdiagonal at 
                              * (branch eq,branch eq) */
    unsigned INDindGiven : 1;   /* flag to indicate inductance was specified */
    unsigned INDicGiven  : 1;   /* flag to indicate init. cond. was specified */
    int  INDsenParmNo;   /* parameter # for sensitivity use;
            set equal to  0 if not a design parameter*/

} INDinstance ;


/* per model data */

typedef struct sINDmodel {       /* model structure for an inductor */
    int INDmodType; /* type index of this device type */
    struct sINDmodel *INDnextModel; /* pointer to next possible model in 
                                     * linked list */
    INDinstance * INDinstances; /* pointer to list of instances that have this
                                 * model */
    IFuid INDmodName;       /* pointer to character string naming this model */
} INDmodel;


#ifdef MUTUAL

        /* structures used to describe mutual inductors */


/* information needed for each instance */

typedef struct sMUTinstance {
    struct sMUTmodel *MUTmodPtr;    /* backpointer to model */
    struct sMUTinstance *MUTnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid MUTname;  /* pointer to character string naming this instance */
    double MUTcoupling;     /* mutual inductance input by user */
    double MUTfactor;       /* mutual inductance scaled for internal use */
    IFuid MUTindName1;  /* name of coupled inductor 1 */
    IFuid MUTindName2;  /* name of coupled inductor 2 */
    INDinstance *MUTind1;   /* pointer to coupled inductor 1 */
    INDinstance *MUTind2;   /* pointer to coupled inductor 2 */
    double *MUTbr1br2;  /* pointers to off-diagonal intersections of */
    double *MUTbr2br1;  /* current branch equations in matrix */

    unsigned MUTindGiven : 1;   /* flag to indicate inductance was specified */
    int  MUTsenParmNo;   /* parameter # for sensitivity use;
            set equal to  0 if not a design parameter*/


} MUTinstance ;


/* per model data */

typedef struct sMUTmodel {       /* model structure for a mutual inductor */
    int MUTmodType; /* type index of this device type */
    struct sMUTmodel *MUTnextModel; /* pointer to next possible model in 
                                     * linked list */
    MUTinstance * MUTinstances; /* pointer to list of instances that have this
                                 * model */
    IFuid MUTmodName;       /* pointer to character string naming this model */
} MUTmodel;

#endif /*MUTUAL*/

/* device parameters */
#define IND_IND 1
#define IND_IC 2
#define IND_FLUX 3
#define IND_VOLT 4
#define IND_IND_SENS 5
#define IND_CURRENT 6
#define IND_POWER 7

/* model parameters */

/* device questions */
#define IND_QUEST_SENS_REAL      201
#define IND_QUEST_SENS_IMAG      202
#define IND_QUEST_SENS_MAG       203
#define IND_QUEST_SENS_PH        204
#define IND_QUEST_SENS_CPLX      205
#define IND_QUEST_SENS_DC        206

#ifdef MUTUAL
/* device parameters */
#define MUT_COEFF 401
#define MUT_IND1 402
#define MUT_IND2 403
#define MUT_COEFF_SENS 404

/* model parameters */

/* device questions */
#define MUT_QUEST_SENS_REAL      601
#define MUT_QUEST_SENS_IMAG      602
#define MUT_QUEST_SENS_MAG       603
#define MUT_QUEST_SENS_PH        604
#define MUT_QUEST_SENS_CPLX      605
#define MUT_QUEST_SENS_DC        606

#endif /*MUTUAL*/

#ifdef __STDC__
extern int INDacLoad(GENmodel*,CKTcircuit*);
extern int INDask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int INDdelete(GENmodel*,IFuid,GENinstance**);
extern void INDdestroy(GENmodel**);
extern int INDload(GENmodel*,CKTcircuit*);
extern int INDmDelete(GENmodel**,IFuid,GENmodel*);
extern int INDparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int INDpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int INDsAcLoad(GENmodel*,CKTcircuit*);
extern int INDsLoad(GENmodel*,CKTcircuit*);
extern void INDsPrint(GENmodel*,CKTcircuit*);
extern int INDsSetup(SENstruct*,GENmodel*);
extern int INDsUpdate(GENmodel*,CKTcircuit*);
extern int INDsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int INDtrunc(GENmodel*,CKTcircuit*,double*);
#else /* stdc */
extern int INDacLoad();
extern int INDask();
extern int INDdelete();
extern void INDdestroy();
extern int INDload();
extern int INDmDelete();
extern int INDparam();
extern int INDpzLoad();
extern int INDsAcLoad();
extern int INDsLoad();
extern void INDsPrint();
extern int INDsSetup();
extern int INDsUpdate();
extern int INDsetup();
extern int INDtrunc();
#endif /* stdc */


#ifdef MUTUAL

#ifdef __STDC__
extern int MUTacLoad(GENmodel*,CKTcircuit*);
extern int MUTask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int MUTdelete(GENmodel*,IFuid,GENinstance**);
extern void MUTdestroy(GENmodel**);
extern int MUTmDelete(GENmodel**,IFuid,GENmodel*);
extern int MUTparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int MUTpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern void MUTsPrint(GENmodel*,CKTcircuit*);
extern int MUTsSetup(SENstruct*,GENmodel*);
extern int MUTsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
#else /* stdc */
extern int MUTacLoad();
extern int MUTask();
extern int MUTdelete();
extern void MUTdestroy();
extern int MUTmDelete();
extern int MUTparam();
extern int MUTpzLoad();
extern void MUTsPrint();
extern int MUTsSetup();
extern int MUTsetup();
#endif /* stdc */


#endif /*MUTUAL*/
#endif /*IND*/
