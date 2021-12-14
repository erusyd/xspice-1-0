/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTmodCrt(type,name,ckt,fast)
     *  Create a device model of the specified type, with the given name
     *  in the named circuit.
     */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "CKTdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTmodCrt.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:50 $")

int
CKTmodCrt(ckt,type,modfast,name)
    GENERIC *ckt;
    int type;
    GENERIC **modfast;
    IFuid name;
{
    extern SPICEdev *DEVices[];
    GENmodel *mymodfast = NULL;
    int error;

    error = CKTfndMod(ckt,&type,(GENERIC**)&mymodfast,name);
    if(error == E_NOMOD) {
        mymodfast = (GENmodel *)MALLOC((*DEVices[type]).DEVmodSize);
        if(mymodfast == (GENmodel *)NULL) return(E_NOMEM);
        mymodfast->GENmodType = type;
        mymodfast->GENmodName = name;
        mymodfast->GENnextModel =(GENmodel *)((CKTcircuit *)ckt)->CKThead[type];
        ((CKTcircuit *)ckt)->CKThead[type]=(GENmodel *)mymodfast;
        if(modfast) *modfast=(GENERIC *)mymodfast;
        return(OK);
    } else if (error==0) {
        if(modfast) *modfast=(GENERIC *)mymodfast;
        return(E_EXISTS);
    } else {
        return(error);
    }
    /*NOTREACHED*/
}
