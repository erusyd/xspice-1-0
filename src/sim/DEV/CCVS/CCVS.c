/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "DEVdefs.h"
#include "CCVSdefs.h"
#include "suffix.h"

RCSID("CCVS.c $Revision: 1.4 $ on $Date: 90/11/09 16:33:58 $")

/* current controlled voltage source */
static IFparm CCVSpTable[] = { /* parameters */ 
 IOP("gain",     CCVS_TRANS,   IF_REAL    ,"Transresistance (gain)"),
 IOP("control",  CCVS_CONTROL, IF_INSTANCE,"Controlling voltage source"),
 IP("sens_trans",CCVS_TRANS_SENS,IF_FLAG,  
        "flag to request sens. WRT transimpedance"),
 OP("pos_node", CCVS_POS_NODE,IF_INTEGER, "Positive node of source"),
 OP("neg_node", CCVS_NEG_NODE,IF_INTEGER, "Negative node of source"),
 OP("c",        CCVS_CURRENT, IF_REAL,    "Device current"),
 OP("p",        CCVS_POWER,   IF_REAL,    "Device power"),
 OP("sens_dc",   CCVS_QUEST_SENS_DC,  IF_REAL,   "dc sensitivity "),
 OP("sens_real", CCVS_QUEST_SENS_REAL,IF_REAL,   "real part of ac sensitivity"),
 OP("sens_imag", CCVS_QUEST_SENS_IMAG,IF_REAL,   "imag part of ac sensitivity"),
 OP("sens_mag",  CCVS_QUEST_SENS_MAG, IF_REAL,   "sensitivity of ac magnitude"),
 OP("sens_ph",   CCVS_QUEST_SENS_PH,  IF_REAL,   "sensitivity of ac phase"),
 OP("sens_cplx", CCVS_QUEST_SENS_CPLX,IF_COMPLEX,"ac sensitivity")
};

/*static IFparm CCVSmPTable[] = { /* model parameters */
/*}; /**/

static char *CCVSnames[] = {
    "H+",
    "H-"
};

SPICEdev CCVSinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "CCVS",
        "Linear current controlled current source",

        sizeof(CCVSnames)/sizeof(char *),
        sizeof(CCVSnames)/sizeof(char *),
        CCVSnames,

        sizeof(CCVSpTable)/sizeof(IFparm),
        CCVSpTable,

        0/*sizeof(CCVSmPTable)/sizeof(IFparm) */,
        NULL/*CCVSmPTable*/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    CCVSparam,
    NULL,
    CCVSload,
    CCVSsetup,
    CCVSsetup,
    NULL,
    NULL,
    CCVSfindBr,
    CCVSload,   /* ac and normal load functions identical */
    NULL,
    CCVSdestroy,
#ifdef DELETES
    CCVSmDelete,
    CCVSdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    CCVSask,
    NULL,
    CCVSpzLoad,
    NULL,
#ifndef NOSENS
    CCVSsSetup,
    CCVSsLoad,
    NULL,
    CCVSsAcLoad,
    CCVSsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(CCVSinstance),
    sizeof(CCVSmodel),

};
