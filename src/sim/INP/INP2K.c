/*
 * Copyright (c) 1988 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "INPdefs.h"
#include "INPmacs.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
#include "suffix.h"

RCSID("INP2K.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:11 $")

void
INP2K(ckt,tab,current)
    GENERIC *ckt;
    INPtables *tab;
    card *current;

{

/* Kname Lname Lname <val> */

int type;   /* the type the model says it is */
char *line; /* the part of the current line left to parse */
char *name; /* the resistor's name */
int error;      /* error code temporary */
GENERIC *fast;  /* pointer to the actual instance */
IFvalue ptemp;  /* a value structure to package resistance into */
IFvalue *parm;  /* ptr to a value structure for function return values */
int waslead;    /* flag to indicate that funny unlabeled number was found */
double leadval; /* actual value of unlabeled number */
IFuid uid;      /* uid for default model */

    line = current->line;
    type = INPtypelook("mutual");
    if(type < 0 ) {
        LITERR("Device type mutual not supported by this binary\n")
        return;
    }
    INPgetTok(&line,&name,1);
    INPinsert(&name,tab);
    if(!tab->defKmod) {
        /* create deafult K model */
        IFnewUid(ckt,&uid,(IFuid)NULL,"K",UID_MODEL,(GENERIC**)NULL);
        IFC(newModel,(ckt,type,&(tab->defKmod),uid))
    }
    IFC(newInstance,(ckt,tab->defKmod,&fast,name))

    parm = INPgetValue(ckt,&line,IF_INSTANCE,tab);
    GCA(INPpName,("inductor1",parm,ckt,type,fast))
    parm = INPgetValue(ckt,&line,IF_INSTANCE,tab);
    GCA(INPpName,("inductor2",parm,ckt,type,fast))

    PARSECALL((&line,ckt,type,fast,&leadval,&waslead,tab))
    if(waslead) {
        ptemp.rValue = leadval;
        GCA(INPpName,("coefficient",&ptemp,ckt,type,fast))
    }
}
