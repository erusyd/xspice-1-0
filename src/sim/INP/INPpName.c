/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * INPpName()
     *
     *  Take a parameter by Name and set it on the specified device 
     */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CPdefs.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "suffix.h"

RCSID("INPpName.c $Revision: 1.1 $ on $Date: 91/04/02 11:57:03 $")

int
INPpName(parm,val,ckt,dev,fast)
    char *parm;     /* the name of the parameter to set */
    IFvalue *val;   /* the parameter union containing the value to set */
    GENERIC *ckt;   /* the circuit this device is a member of */
    int dev;        /* the device type code to the device being parsed */
    GENERIC *fast;  /* direct pointer to device being parsed */

{
    int error;  /* int to store evaluate error return codes in */
    int i;

    for(i=0;i<(*(ft_sim->devices)[dev]).numInstanceParms;i++) {
        if(strcmp(parm,
                ((*(ft_sim->devices)[dev]).instanceParms[i].keyword))==0) {
            error = (*(ft_sim->setInstanceParm))(ckt,fast, (*(ft_sim->
                    devices)[dev]).instanceParms[i].id,val,(IFvalue*)NULL);
            if(error) return(error);
            break;
        }
    }
    if(i==(*(ft_sim->devices)[dev]).numInstanceParms) {
        return(E_BADPARM);
    }
    return(OK);
}
