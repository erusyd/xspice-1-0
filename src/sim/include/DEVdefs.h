/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef DEV
#define DEV "DEVdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:25:36 $ "

#include "OPTdefs.h"
#include "GENdefs.h"
#include "IFsim.h"
#include "CKTdefs.h"
#include "complex.h"

#ifdef __STDC__
double DEVlimvds(double,double);
double DEVpnjlim(double,double,double,double,int*);
double DEVfetlim(double,double,double);
void DEVcmeyer(double,double,double,double,double,double,double,double,double,
        double,double,double*,double*,double*,double,double,double,double);
void DEVqmeyer(double,double,double,double,double,double*,double*,double*,
        double,double);
void DEVcap(CKTcircuit*, double, double, double, double, double, double,
        double, double, double, double, double, double, double, double,
        double*, double*, double*, double*, double*, double*, double*,
        double*, double*, double*, double*, double*, double, double,
        double, double*, double*, double);
double DEVpred(CKTcircuit*,int);
#else /* stdc */
double DEVlimvds();
double DEVpnjlim();
double DEVfetlim();
void DEVcmeyer();
void DEVqmeyer();
void DEVcap();
double DEVpred();
#endif /* stdc */

typedef struct SPICEdev {
    IFdevice DEVpublic;

#ifdef __STDC__
    int (*DEVparam)(int,IFvalue*,GENinstance*,IFvalue *);  
        /* routine to input a parameter to a device instance */
    int (*DEVmodParam)(int,IFvalue*,GENmodel*);   
        /* routine to input a paramater to a model */
    int (*DEVload)(GENmodel*,CKTcircuit*);   
        /* routine to load the device into the matrix */
    int (*DEVsetup)(SMPmatrix*,GENmodel*,CKTcircuit*,int*);  
        /* setup routine to preprocess devices once before soloution begins */
    int (*DEVpzSetup)(SMPmatrix*,GENmodel*,CKTcircuit*,int*);  
        /* setup routine to process devices specially for pz analysis */
    int (*DEVtemperature)(GENmodel*,CKTcircuit*);    
        /* subroutine to do temperature dependent setup processing */
    int (*DEVtrunc)(GENmodel*,CKTcircuit*,double*);  
        /* subroutine to perform truncation error calc. */
    int (*DEVfindBranch)(CKTcircuit*,GENmodel*,IFuid); 
        /* subroutine to search for device branch eq.s */
    int (*DEVacLoad)(GENmodel*,CKTcircuit*); 
        /* ac analysis loading function */
    int (*DEVaccept)(CKTcircuit*,GENmodel*); 
        /* subroutine to call on acceptance of a timepoint */
    void (*DEVdestroy)(GENmodel**);   
        /* subroutine to destroy all models and instances */
    int (*DEVmodDelete)(GENmodel**,IFuid,GENmodel*);  
        /* subroutine to delete a model and all instances */
    int (*DEVdelete)(GENmodel*,IFuid,GENinstance**); 
        /* subroutine to delete an instance */
    int (*DEVsetic)(GENmodel*,CKTcircuit*);  
        /* routine to pick up device init conds from rhs */
    int (*DEVask)(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);    
        /* routine to ask about device details*/
    int (*DEVmodAsk)(CKTcircuit*,GENmodel*,int,IFvalue*); 
        /* routine to ask about model details*/
    int (*DEVpzLoad)(GENmodel*,CKTcircuit*,SPcomplex*); 
        /* routine to load for pole-zero analysis */
    int (*DEVconvTest)(GENmodel*,CKTcircuit*);  
        /* convergence test function */

    int (*DEVsenSetup)(SENstruct*,GENmodel*);
        /* routine to setup the device sensitivity info */
    int (*DEVsenLoad)(GENmodel*,CKTcircuit*);   
        /* routine to load the device sensitivity info */
    int (*DEVsenUpdate)(GENmodel*,CKTcircuit*); 
        /* routine to update the device sensitivity info */
    int (*DEVsenAcLoad)(GENmodel*,CKTcircuit*); 
        /* routine to load  the device ac sensitivity info */
    void (*DEVsenPrint)(GENmodel*,CKTcircuit*);   
        /* subroutine to print out sensitivity info */
    int (*DEVsenTrunc)(GENmodel*,CKTcircuit*,double*);   
        /* subroutine to print out sensitivity info */

#else /* stdc */

    int (*DEVparam)();  /* routine to input a parameter to a device instance */
    int (*DEVmodParam)();   /* routine to input a paramater to a model */
    int (*DEVload)();   /* routine to load the device into the matrix */
    int (*DEVsetup)();  /* setup routine to preprocess devices once before
                         * soloution begins */
    int (*DEVpzSetup)();  /* setup routine to process devices specially for 
                         * pz analysis */
    int (*DEVtemperature)();    /* subroutine to do temperature dependent 
                                 * setup processing */
    int (*DEVtrunc)();  /* subroutine to perform truncation error calc. */
    int (*DEVfindBranch)(); /* subroutine to search for device branch eq.s */
    int (*DEVacLoad)(); /* ac analysis loading function */
    int (*DEVaccept)(); /* subroutine to call on acceptance of a timepoint */
    void (*DEVdestroy)();   /* subroutine to destroy all models and instances */
    int (*DEVmodDelete)();  /* subroutine to delete a model and all instances */
    int (*DEVdelete)(); /* subroutine to delete an instance */
    int (*DEVsetic)();  /* routine to pick up device init conds from rhs */
    int (*DEVask)();    /* routine to ask about device details*/
    int (*DEVmodAsk)(); /* routine to ask about model details*/
    int (*DEVpzLoad)(); /* routine to load for pole-zero analysis */
    int (*DEVconvTest)();   /* convergence test function */

    int (*DEVsenSetup)();   /* routine to setup the device sensitivity info */
    int (*DEVsenLoad)();    /* routine to load the device sensitivity info */
    int (*DEVsenUpdate)();  /* routine to update the device sensitivity info */
    int (*DEVsenAcLoad)();  /* routine to load  the device ac sensitivity info*/
    void (*DEVsenPrint)();   /* subroutine to print out sensitivity info */
    int (*DEVsenTrunc)();   /* subroutine to print out sensitivity info */

#endif /* stdc */

    int DEVinstSize;    /* size of an instance */
    int DEVmodSize;     /* size of a model */

} SPICEdev;  /* instance of structure for each possible type of
                         * device */

/*
 * these are for the IBM PC which gets upset by the long description 
 * strings used in IFparm definitions because they add up to more than
 * 64k of static data
 */
#ifndef MINDATA
#define IP(a,b,c,d) { a , b , c|IF_SET , d }
#define OP(a,b,c,d) { a , b , c|IF_ASK , d }
#define IOP(a,b,c,d) { a , b , c|IF_SET|IF_ASK , d }
#define P(a,b,c,d) { a , b , c , d }
#else /* MINDATA */
#define IP(a,b,c,d) { a , b , c|IF_SET , "" }
#define OP(a,b,c,d) { a , b , c|IF_ASK , "" }
#define IOP(a,b,c,d) { a , b , c|IF_SET|IF_ASK , "" }
#define P(a,b,c,d) { a , b , c , "" }
#endif /* MINDATA */

#endif /*DEV*/
