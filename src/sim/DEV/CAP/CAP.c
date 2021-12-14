/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "CAPdefs.h"
#include "suffix.h"

RCSID("CAP.c $Revision: 1.4 $ on $Date: 90/11/09 16:33:53 $")

static IFparm CAPpTable[] = { /* parameters */ 
 IOP("capacitance", CAP_CAP,    IF_REAL, "Device capacitance"),
 IOP("ic",          CAP_IC,     IF_REAL, "Initial capacitor voltage"),
 IOP("w",           CAP_WIDTH,  IF_REAL, "Device width"),
 IOP("l",           CAP_LENGTH, IF_REAL, "Device length"),
 IP("sens_cap",   CAP_CAP_SENS, IF_FLAG, "flag to request sens. WRT cap."),
 OP("c",            CAP_CURRENT,IF_REAL, "Device current"),
 OP("p",            CAP_POWER,  IF_REAL, "Instantaneous device power"),
 OP("sens_dc",      CAP_QUEST_SENS_DC, IF_REAL,    "dc sensitivity "),
 OP("sens_real", CAP_QUEST_SENS_REAL, IF_REAL, "real part of ac sensitivity"),
 OP("sens_imag",CAP_QUEST_SENS_IMAG,IF_REAL,"dc sens. & imag part of ac sens."),
 OP("sens_mag",  CAP_QUEST_SENS_MAG,  IF_REAL, "sensitivity of ac magnitude"),
 OP("sens_ph",   CAP_QUEST_SENS_PH,   IF_REAL, "sensitivity of ac phase"),
 OP("sens_cplx", CAP_QUEST_SENS_CPLX, IF_COMPLEX, "ac sensitivity")
};

static IFparm CAPmPTable[] = { /* names of model parameters */
 IOP( "cj",     CAP_MOD_CJ,      IF_REAL, "Bottom Capacitance per area"),
 IOP( "cjsw",   CAP_MOD_CJSW,    IF_REAL, "Sidewall capacitance per meter"),
 IOP( "defw",   CAP_MOD_DEFWIDTH,IF_REAL, "Default width"),
 IP ( "c",      CAP_MOD_C,       IF_FLAG, "Capacitor model"),
 IOP( "narrow", CAP_MOD_NARROW,  IF_REAL, "width correction factor")
};

static char *CAPnames[] = {
    "C+",
    "C-"
};


SPICEdev CAPinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {   "Capacitor",
        "Fixed capacitor",

        sizeof(CAPnames)/sizeof(char *),
        sizeof(CAPnames)/sizeof(char *),
        CAPnames,

        sizeof(CAPpTable)/sizeof(IFparm),
        CAPpTable,

        sizeof(CAPmPTable)/sizeof(IFparm),
        CAPmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    CAPparam,
    CAPmParam,
    CAPload,
    CAPsetup,
    CAPsetup,
    CAPtemp,
    CAPtrunc,
    NULL,
    CAPacLoad,
    NULL,
    CAPdestroy,
#ifdef DELETES
    CAPmDelete,
    CAPdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif  /* DELETES */
    CAPgetic,
    CAPask,
    CAPmAsk,
    CAPpzLoad,
    NULL,
#ifndef NOSENS
    CAPsSetup,
    CAPsLoad,
    CAPsUpdate,
    CAPsAcLoad,
    CAPsPrint,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */
    NULL,

    sizeof(CAPinstance),
    sizeof(CAPmodel),


};

