/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "RESdefs.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "suffix.h"

RCSID("RES.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:28 $")

static IFparm RESpTable[] = { /* parameters */ 
 IOP( "resistance", RES_RESIST, IF_REAL,"Resistance"),
 IOP( "w",          RES_WIDTH,  IF_REAL,"Width"),
 IOP( "l",          RES_LENGTH, IF_REAL,"Length"),
 IOP( "c",          RES_CURRENT,IF_REAL,"Current"),
 IOP( "p",          RES_POWER,  IF_REAL,"Power"),
 IP("sens_resist", RES_RESIST_SENS, IF_FLAG,   
        "flag to request sensitivity WRT resistance"),
 OP("sens_dc",  RES_QUEST_SENS_DC,  IF_REAL,    "dc sensitivity "),
 OP("sens_real",RES_QUEST_SENS_REAL,IF_REAL,    
        "dc sensitivity and real part of ac sensitivity"),
 OP("sens_imag",RES_QUEST_SENS_IMAG,IF_REAL,    
        "dc sensitivity and imag part of ac sensitivity"),
 OP("sens_mag", RES_QUEST_SENS_MAG, IF_REAL,    "ac sensitivity of magnitude"),
 OP("sens_ph",  RES_QUEST_SENS_PH,  IF_REAL,    "ac sensitivity of phase"),
 OP("sens_cplx",RES_QUEST_SENS_CPLX,IF_COMPLEX,    "ac sensitivity"),
 IOP("temp",        RES_TEMP,   IF_REAL,"Instance operating temperature"),
} ;
static IFparm RESmPTable[] = { /* model parameters */
 IOP( "tc1",    RES_MOD_TC1,      IF_REAL,"First order temp. coefficient"),
 IOP( "tc2",    RES_MOD_TC2,      IF_REAL,"Second order temp. coefficient"),
 IOP( "rsh",    RES_MOD_RSH,      IF_REAL,"Sheet resistance"),
 IOP( "defw",   RES_MOD_DEFWIDTH, IF_REAL,"Default device width"),
 IP( "r",      RES_MOD_R,        IF_FLAG,"Device is a resistor model"),
 IOP( "narrow", RES_MOD_NARROW,   IF_REAL,"Narrowing of resistor"),
 IOP("tnom",    RES_MOD_TNOM,     IF_REAL,"Parameter measurement temperature"),
};

static char *RESnames[] = {
    "R+",
    "R-"
};

SPICEdev RESinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Resistor",
        "Simple linear resistor",

        sizeof(RESnames)/sizeof(char *),
        sizeof(RESnames)/sizeof(char *),
        RESnames,

        sizeof(RESpTable)/sizeof(IFparm),
        RESpTable,

        sizeof(RESmPTable)/sizeof(IFparm),
        RESmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    RESparam,
    RESmParam,
    RESload,
    RESsetup,
    RESsetup,
    REStemp,
    NULL,
    NULL,
    RESload,  /* ac load and normal load are identical */
    NULL,
    RESdestroy,
#ifdef DELETES
    RESmDelete,
    RESdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    RESask,
    NULL,
    RESpzLoad,
    NULL,
#ifndef NOSENS
    RESsSetup,
    RESsLoad,
    NULL,
    RESsAcLoad,
    RESsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(RESinstance),
    sizeof(RESmodel),

};
