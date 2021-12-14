/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * CKTpModName()
     *
     *  Take a parameter by Name and set it on the specified model 
     */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "IFsim.h"
#include "DEVdefs.h"
#include "CKTdefs.h"
#include "GENdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTpModName.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:04 $")

extern SPICEdev *DEVices[];

/* ARGSUSED */
int
CKTpModName(parm,val,ckt,type,name,modfast)
    char *parm; /* the name of the parameter to set */
    IFvalue *val; /* the parameter union containing the value to set */
    CKTcircuit *ckt;/* the circuit this model is a member of */
    int type;        /* the device type code to the model being parsed */
    IFuid name;     /* the name of the model being parsed */
    GENmodel **modfast;    /* direct pointer to model being parsed */

{
    int error;  /* int to store evaluate error return codes in */
    int i;

    for(i=0;i<(*DEVices[type]).DEVpublic.numModelParms;i++) {
        if(strcmp(parm,((*DEVices[type]).DEVpublic.modelParms[i].keyword))==0){
            error = CKTmodParam((GENERIC *)ckt,(GENERIC*)*modfast,
                    (*DEVices[type]).DEVpublic.modelParms[i].id,val,
                    (IFvalue*)NULL);
            if(error) return(error);
            break;
        }
    }
    if(i==(*DEVices[type]).DEVpublic.numModelParms) {
        return(E_BADPARM);
    }
    return(OK);
}
