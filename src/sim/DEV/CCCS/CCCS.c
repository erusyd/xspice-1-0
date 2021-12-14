/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "DEVdefs.h"
#include "CCCSdefs.h"
#include "suffix.h"

RCSID("CCCS.c $Revision: 1.4 $ on $Date: 90/11/09 16:33:55 $")

static IFparm CCCSpTable[] = { /* parameters */ 
 IOP("gain",    CCCS_GAIN,    IF_REAL    ,"Gain of source"),
 IOP("control", CCCS_CONTROL, IF_INSTANCE,"Name of controlling source"),
 IP("sens_gain",CCCS_GAIN_SENS,IF_FLAG, "flag to request sensitivity WRT gain"),
 OP("neg_node", CCCS_NEG_NODE,IF_INTEGER, "Negative node of source"),
 OP("pos_node", CCCS_POS_NODE,IF_INTEGER, "Positive node of source"),
 OP("c",        CCCS_CURRENT, IF_REAL,    "CCCS current"),
 OP("p",        CCCS_POWER,   IF_REAL,    "CCCS current"),
 OP("sens_dc",  CCCS_QUEST_SENS_DC,  IF_REAL,    "dc sensitivity "),
 OP("sens_real",CCCS_QUEST_SENS_REAL,IF_REAL,    "real part of ac sensitivity"),
 OP("sens_imag",CCCS_QUEST_SENS_IMAG,IF_REAL,    "imag part of ac sensitivity"),
 OP("sens_mag", CCCS_QUEST_SENS_MAG, IF_REAL,    "sensitivity of ac magnitude"),
 OP("sens_ph",  CCCS_QUEST_SENS_PH,  IF_REAL,    "sensitivity of ac phase"),
 OP("sens_cplx",CCCS_QUEST_SENS_CPLX,IF_COMPLEX, "ac sensitivity")

};

/* static IFparm CCCSmPTable[] = { /* model parameters */ 
/*};*/

static char *CCCSnames[] = {
    "F+",
    "F-"
};

SPICEdev CCCSinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {   "CCCS",
        "Current controlled current source",

        sizeof(CCCSnames)/sizeof(char *),
        sizeof(CCCSnames)/sizeof(char *),
        CCCSnames,

        sizeof(CCCSpTable)/sizeof(IFparm),
        CCCSpTable,

        0/*sizeof(CCCSmPTable)/sizeof(IFparm)*/,
        0/*CCCSmPTable*/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    CCCSparam,
    NULL,
    CCCSload,
    CCCSsetup,
    CCCSsetup,
    NULL,
    NULL,
    NULL,
    CCCSload,
    NULL,
    CCCSdestroy,
#ifdef DELETES
    CCCSmDelete,
    CCCSdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    CCCSask,
    NULL,
    CCCSpzLoad,
    NULL,
#ifndef NOSENS
    CCCSsSetup,
    CCCSsLoad,
    NULL,
    CCCSsAcLoad,
    CCCSsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(CCCSinstance),
    sizeof(CCCSmodel),

};
