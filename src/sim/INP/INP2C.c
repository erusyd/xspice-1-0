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

RCSID("INP2C.c $Revision: 1.1 $ on $Date: 91/04/02 11:55:58 $")

void
INP2C(ckt,tab,current)
    GENERIC *ckt;
    INPtables *tab;
    card *current;

{

/* parse a capacitor card */
/* Cname <node> <node> <val> [IC=<val>] */

int mytype; /* the type we determine resistors are */
int type;   /* the type the model says it is */
char *line; /* the part of the current line left to parse */
char *name; /* the resistor's name */
char *model;    /* the name of the resistor's model */
char *nname1;   /* the first node's name */
char *nname2;   /* the second node's name */
GENERIC *node1; /* the first node's node pointer */
GENERIC *node2; /* the second node's node pointer */
double val;     /* temp to held resistance */
int error;      /* error code temporary */
INPmodel *thismodel;    /* pointer to model structure describing our model */
GENERIC *mdfast;    /* pointer to the actual model */
GENERIC *fast;  /* pointer to the actual instance */
IFvalue ptemp;  /* a value structure to package resistance into */
int waslead;    /* flag to indicate that funny unlabeled number was found */
double leadval; /* actual value of unlabeled number */
IFuid uid;      /* uid for default cap model */

    mytype = INPtypelook("Capacitor");
    if(mytype < 0 ) {
        LITERR("Device type Capacitor not supported by this binary\n")
        return;
    }
    line = current->line;
    INPgetTok(&line,&name,1);
    INPinsert(&name,tab);
    INPgetTok(&line,&nname1,1);
    INPtermInsert(ckt,&nname1,tab,&node1);
    INPgetTok(&line,&nname2,1);
    INPtermInsert(ckt,&nname2,tab,&node2);
    val = INPevaluate(&line,&error,1);
    if(error == 0)  {  /* Looks like a number */
        type = mytype;
        ptemp.rValue = val;
        if(!tab->defCmod) {
            IFnewUid(ckt,&uid,(IFuid)NULL,"C",UID_MODEL,(GENERIC**)NULL);
            IFC(newModel,(ckt,type,&(tab->defCmod),uid))
        }
        IFC(newInstance,(ckt,tab->defCmod,&fast,name))
        GCA(INPpName,("capacitance",&ptemp,ckt,type,fast))
    }  else  { /* looks like character strings */
        INPgetTok(&line,&model,1);
        INPinsert(&model,tab);
        thismodel = (INPmodel *)NULL;
        current->error = INPgetMod(ckt,model,&thismodel,tab);
        if(thismodel != NULL) {
            if(mytype != thismodel->INPmodType) {
                LITERR("incorrect model type")
                return;
            }
            type = mytype;
            mdfast = thismodel->INPmodfast;
        } else {
            type = mytype;
            if(!tab->defCmod) {
                IFnewUid(ckt,&uid,(IFuid)NULL,"C",UID_MODEL,(GENERIC**)NULL);
                IFC(newModel,(ckt,type,&(tab->defCmod),uid))
            }
            mdfast = tab->defCmod;
        }
        IFC(newInstance,(ckt,mdfast,&fast,name))
    }
    IFC(bindNode,(ckt,fast,1,node1))
    IFC(bindNode,(ckt,fast,2,node2))
    PARSECALL((&line,ckt,type,fast,&leadval,&waslead,tab))
    if(waslead) {
        ptemp.rValue = leadval;
        GCA(INPpName,("capacitance",&ptemp,ckt,type,fast))
    }
}
