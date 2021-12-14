/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "util.h"
#include "INPdefs.h"
#include <string.h>
#include "suffix.h"

RCSID("INPdoModel.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:39 $")

/*ARGSUSED*/
char *
INPdomodel(ckt,image,tab)
    GENERIC *ckt;
    card *image;
    INPtables *tab;
{

    char *modname;
    int type;
    int lev;
    char *typename;
    char *err = (char *)NULL;
    char *line;

    line = image->line;
    INPgetTok(&line,&modname,1);  /* throw away '.model' */
    INPgetTok(&line,&modname,1);
    INPinsert(&modname,tab);
    INPgetTok(&line,&typename,1);
    if( (strcmp(typename,"npn") == 0)||(strcmp(typename,"pnp")==0) ) {
        type = INPtypelook("BJT");
        if(type < 0) {
            err = INPmkTemp("Device type BJT not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if(strcmp(typename,"d") == 0) {
        type = INPtypelook("Diode");
        if(type < 0) {
            err = INPmkTemp("Device type Diode not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if( (strcmp(typename,"njf") == 0) || (strcmp(typename,"pjf") == 0)){
        type = INPtypelook("JFET");
        if(type < 0) {
            err = INPmkTemp("Device type JFET not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if( (strcmp(typename,"nmf") == 0) || (strcmp(typename,"pmf")==0) ) {
        type = INPtypelook("MES");
        if(type < 0) {
            err = INPmkTemp("Device type MES not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if(strcmp(typename,"urc") == 0) {
        type = INPtypelook("URC");
        if(type < 0) {
            err = INPmkTemp("Device type URC not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if( (strcmp(typename,"nmos")==0) || (strcmp(typename,"pmos")==0) ) {
        err = INPfindLev(line,&lev);
        switch(lev) {
            case 1:
            default:
                type = INPtypelook("Mos1");
                if(type < 0) {
                    err = INPmkTemp(
                            "Device type MOS1 not available in this binary\n");
                }
                break;
            case 2:
                type = INPtypelook("Mos2");
                if(type < 0) {
                    err = INPmkTemp(
                            "Device type MOS2 not available in this binary\n");
                }
                break;
            case 3:
                type = INPtypelook("Mos3");
                if(type < 0) {
                    err = INPmkTemp(
                            "Device type MOS3 not available in this binary\n");
                }
                break;
            case 4:
                type = INPtypelook("BSIM");
                if(type < 0) {
                    err = INPmkTemp(
                            "Device type BSIM not available in this binary\n");
                }
                break;
            case 6:
                type = INPtypelook("Mos6");
                if(type < 0) {
                    err = INPmkTemp(
                            "Device type MOS6 not available in this binary\n");
                }
                break;
        }
        INPmakeMod(modname,type,image);
    } else if(strcmp(typename,"r") == 0) {
        type = INPtypelook("Resistor");
        if(type < 0) {
            err = INPmkTemp(
                    "Device type Resistor not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if(strcmp(typename,"c") == 0) {
        type = INPtypelook("Capacitor");
        if(type < 0) {
            err = INPmkTemp(
                    "Device type Capacitor not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if(strcmp(typename,"sw") == 0) {
        type = INPtypelook("Switch");
        if(type < 0) {
            err = INPmkTemp(
                    "Device type Switch not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else if(strcmp(typename,"csw") == 0) {
        type = INPtypelook("CSwitch");
        if(type < 0) {
            err = INPmkTemp(
                    "Device type CSwitch not available in this binary\n");
        }
        INPmakeMod(modname,type,image);
    } else {

/* gtri - modify - wbk - 10/23/90 - modify to look for code models */

        /* comment out old code */
#if 0
        type = -1;
        err = (char *)MALLOC(35 + strlen(typename));
        (void)sprintf(err,"unknown model type %s - ignored\n",typename);
#endif

        /* add new code */

        /* look for this model type and put it in the table of models */
        type = INPtypelook(typename);
        if(type < 0) {
            err = (char *) MALLOC(35 + strlen(typename));
            sprintf(err,"Unknown model type %s - ignored\n",typename);
        }
        else
            INPmakeMod(modname,type,image);

/* gtri - end - wbk - 10/23/90 */

    }
    return(err);
}
