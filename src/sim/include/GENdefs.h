/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef GEN
#define GEN "GENdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:00 $ "

#include "IFsim.h"

        /* definitions used to describe generic devices */

/* information used to describe a single instance */

typedef struct sGENinstance {
    struct sGENmodel *GENmodPtr;    /* backpointer to model */
    struct sGENinstance *GENnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid GENname;  /* pointer to character string naming this instance */
    int GENnode1;   /* appropriate node numbers */
    int GENnode2;   /* appropriate node numbers */
    int GENnode3;   /* appropriate node numbers */
    int GENnode4;   /* appropriate node numbers */
    int GENnode5;   /* appropriate node numbers */
} GENinstance ;

/* per model data */

typedef struct sGENmodel {       /* model structure for a resistor */
    int GENmodType; /* type index of this device type */
    struct sGENmodel *GENnextModel; /* pointer to next possible model in 
                                     * linked list */
    GENinstance * GENinstances; /* pointer to list of instances that have this
                                 * model */
    IFuid GENmodName;       /* pointer to character string naming this model */
} GENmodel;

#endif /*GEN*/
