/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "DIOdefs.h"
#include "suffix.h"

RCSID("DIO.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:03 $")

static IFparm DIOpTable[] = { /* parameters */ 
 IOP("off",    DIO_OFF,    IF_FLAG, "Initially off"),
 IOP("temp",   DIO_TEMP,   IF_REAL, "Instance temperature"),
 IOP("ic",     DIO_IC,     IF_REAL, "Initial device voltage"),
 IOP("area",   DIO_AREA,   IF_REAL, "Area factor"),
 IP("sens_area",DIO_AREA_SENS,IF_FLAG,"flag to request sensitivity WRT area"),
 OP("voltage",DIO_VOLTAGE,IF_REAL, "Diode voltage"),
 OP("current",DIO_CURRENT,IF_REAL, "Diode current"),
 OP("charge", DIO_CHARGE, IF_REAL, "Diode capacitor charge"),
 OP("capcur", DIO_CAPCUR, IF_REAL, "Diode capacitor current"),
 OP("cond",   DIO_CONDUCT,IF_REAL, "Diode conductance"),
 OP("p",      DIO_POWER,  IF_REAL, "Diode power"),
 OP("c",      DIO_CURRENT,IF_REAL, "Diode current"),
 OP("sens_dc",DIO_QUEST_SENS_DC,     IF_REAL,   "dc sensitivity "),
 OP("sens_real", DIO_QUEST_SENS_REAL,IF_REAL,   
        "dc sens. and real part of ac sensitivity"),
 OP("sens_imag", DIO_QUEST_SENS_IMAG,IF_REAL,   "imag part of ac sensitivity "),
 OP("sens_mag",  DIO_QUEST_SENS_MAG, IF_REAL,   "sensitivity of ac magnitude"),
 OP("sens_ph",   DIO_QUEST_SENS_PH,  IF_REAL,   "sensitivity of ac phase"),
 OP("sens_cplx", DIO_QUEST_SENS_CPLX,IF_COMPLEX,"ac sensitivity")
};
static IFparm DIOmPTable[] = { /* model parameters */
 IOP( "is",  DIO_MOD_IS,  IF_REAL, "Saturation current"),
 IOP( "tnom",DIO_MOD_TNOM,IF_REAL, "Parameter measurement temperature"),
 IOP( "rs",  DIO_MOD_RS,  IF_REAL, "Ohmic resistance"),
 IOP( "n",   DIO_MOD_N,   IF_REAL, "Emission Coefficient"),
 IOP( "tt",  DIO_MOD_TT,  IF_REAL, "Transit Time"),
 IOP( "cjo", DIO_MOD_CJO, IF_REAL, "Junction capacitance"),
 IOP( "cj0", DIO_MOD_CJO, IF_REAL, "Junction capacitance"),
 IOP( "vj",  DIO_MOD_VJ,  IF_REAL, "Junction potential"),
 IOP( "m",   DIO_MOD_M,   IF_REAL, "Grading coefficient"),
 IOP( "eg",  DIO_MOD_EG,  IF_REAL, "Activation energy"),
 IOP( "xti", DIO_MOD_XTI, IF_REAL, "Saturation current temperature exp."),
 IOP( "fc",  DIO_MOD_FC,  IF_REAL, "Forward bias junction fit parameter"),
 IOP( "bv",  DIO_MOD_BV,  IF_REAL, "Reverse breakdown voltage"),
 IOP( "ibv", DIO_MOD_IBV, IF_REAL, "Current at reverse breakdown voltage"),
 OP( "cond", DIO_MOD_COND,IF_REAL, "Ohmic conductance"),
 IP( "d",    DIO_MOD_D,   IF_FLAG, "Diode model")
};

static char *DIOnames[] = {
    "D+",
    "D-"
};

SPICEdev DIOinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Diode",
        "Junction Diode model",

        sizeof(DIOnames)/sizeof(char *),
        sizeof(DIOnames)/sizeof(char *),
        DIOnames,

        sizeof(DIOpTable)/sizeof(IFparm),
        DIOpTable,

        sizeof(DIOmPTable)/sizeof(IFparm),
        DIOmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    DIOparam,
    DIOmParam,
    DIOload,
    DIOsetup,
    DIOsetup,
    DIOtemp,
    DIOtrunc,
    NULL,
    DIOacLoad,
    NULL,
    DIOdestroy,
#ifdef DELETES
    DIOmDelete,
    DIOdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    DIOgetic,
    DIOask,
    DIOmAsk,
    DIOpzLoad,
#ifdef NEWCONV
    DIOconvTest,
#else /* NEWCONV */
    NULL,
#endif /* NEWCONV */

#ifndef NOSENS 
    DIOsSetup,
    DIOsLoad,
    DIOsUpdate,
    DIOsAcLoad,
    DIOsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(DIOinstance),
    sizeof(DIOmodel),


};

