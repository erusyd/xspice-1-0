/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "DEVdefs.h"
#include "VCVSdefs.h"
#include "suffix.h"

RCSID("VCVS.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:43 $")

static IFparm VCVSpTable[] = { /* parameters */ 
 IOP("gain", VCVS_GAIN, IF_REAL,"Voltage gain"),
 IP("sens_gain",VCVS_GAIN_SENS,IF_FLAG,"flag to request sensitivity WRT gain"),
 OP("pos_node", VCVS_POS_NODE, IF_INTEGER, "Positive node of source"),
 OP("neg_node", VCVS_NEG_NODE, IF_INTEGER, "Negative node of source"),
 OP("cont_p_node",VCVS_CONT_P_NODE,IF_INTEGER,"Positive node of contr. source"),
 OP("cont_n_node",VCVS_CONT_N_NODE,IF_INTEGER,"Negative node of contr. source"),
 IP("ic", VCVS_IC, IF_REAL, "Initial condition of controlling source"),
 OP("c",        VCVS_CURRENT,       IF_REAL,        "Current"),
 OP("p",        VCVS_POWER,         IF_REAL,        "Power"),
 OP("sens_dc",   VCVS_QUEST_SENS_DC,  IF_REAL, "dc sensitivity "),
 OP("sens_real", VCVS_QUEST_SENS_REAL,IF_REAL, "real part of ac sensitivity"),
 OP("sens_imag", VCVS_QUEST_SENS_IMAG,IF_REAL, "imag part of ac sensitivity"),
 OP("sens_mag",  VCVS_QUEST_SENS_MAG, IF_REAL, "sensitivity of ac magnitude"),
 OP("sens_ph",   VCVS_QUEST_SENS_PH,  IF_REAL, "sensitivity of ac phase"),
 OP("sens_cplx", VCVS_QUEST_SENS_CPLX,     IF_COMPLEX,    "ac sensitivity")
};

/*static IFparm VCVSmPTable[] = { /* model parameters */
/*};/**/

static char *VCVSnames[] = {
    "V+",
    "V-",
    "VC+",
    "VC-"
};

SPICEdev VCVSinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "VCVS",
        "Voltage controlled voltage source",

        sizeof(VCVSnames)/sizeof(char *),
        sizeof(VCVSnames)/sizeof(char *),
        VCVSnames,

        sizeof(VCVSpTable)/sizeof(IFparm),
        VCVSpTable,

        0/*sizeof(VCVSmPTable)/sizeof(IFparm)/**/,
        NULL/*VCVSmPTable/**/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    VCVSparam,
    NULL,
    VCVSload,
    VCVSsetup,
    VCVSsetup,
    NULL,
    NULL,
    VCVSfindBr,
    VCVSload,   /* AC and normal loads are identical */
    NULL,
    VCVSdestroy,
#ifdef DELETES
    VCVSmDelete,
    VCVSdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    VCVSask,
    NULL,
    VCVSpzLoad,
    NULL,
#ifndef NOSENS
    VCVSsSetup,
    VCVSsLoad,
    NULL,
    VCVSsAcLoad,
    VCVSsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(VCVSinstance),
    sizeof(VCVSmodel),

};

