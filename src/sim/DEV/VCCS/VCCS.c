/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "IFsim.h"
#include "VCCSdefs.h"
#include "suffix.h"

RCSID("VCCSdelete.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:41 $")

static IFparm VCCSpTable[] = { /* parameters */ 
 IOP("gain",       VCCS_TRANS, IF_REAL, "Transconductance of source (gain)"),
 IP("sens_trans",  VCCS_TRANS_SENS,IF_FLAG,
        "flag to request sensitivity WRT transconductance"),
 OP("pos_node",    VCCS_POS_NODE, IF_INTEGER, "Positive node of source"),
 OP("neg_node",    VCCS_NEG_NODE, IF_INTEGER, "Negative node of source"),
 OP("cont_p_node",VCCS_CONT_P_NODE,IF_INTEGER,"Positive node of contr. source"),
 OP("cont_n_node",VCCS_CONT_N_NODE,IF_INTEGER,"Negative node of contr. source"),
 IP("ic",          VCCS_IC, IF_REAL, "Initial condition of controlling source"),
 OP("c",            VCCS_CURRENT,IF_REAL, "Current"),
 OP("p",            VCCS_POWER,  IF_REAL, "Power"),
 OP("sens_dc",   VCCS_QUEST_SENS_DC,       IF_REAL,    "dc sensitivity "),
 OP("sens_real", VCCS_QUEST_SENS_REAL, IF_REAL, "real part of ac sensitivity"),
 OP("sens_imag", VCCS_QUEST_SENS_IMAG, IF_REAL, "imag part of ac sensitivity"),
 OP("sens_mag",  VCCS_QUEST_SENS_MAG,  IF_REAL, "sensitivity of ac magnitude"),
 OP("sens_ph",   VCCS_QUEST_SENS_PH,   IF_REAL,  "sensitivity of ac phase"),
 OP("sens_cplx", VCCS_QUEST_SENS_CPLX, IF_COMPLEX,    "ac sensitivity")
};

/*static IFparm VCCSmPTable[] = { /* model parameters */
/*};/**/

static char *VCCSnames[] = {
    "V+",
    "V-",
    "VC+",
    "VC-"
};

SPICEdev VCCSinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "VCCS",
        "Voltage controlled current source",

        sizeof(VCCSnames)/sizeof(char *),
        sizeof(VCCSnames)/sizeof(char *),
        VCCSnames,

        sizeof(VCCSpTable)/sizeof(IFparm),
        VCCSpTable,

        0/*sizeof(VCCSmPTable)/sizeof(IFparm)/**/,
        NULL/*VCCSmPTable/**/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    VCCSparam,
    NULL,
    VCCSload,
    VCCSsetup,
    VCCSsetup,
    NULL,
    NULL,
    NULL,
    VCCSload,   /* ac and normal loads are identical */
    NULL,
    VCCSdestroy,
#ifdef DELETES
    VCCSmDelete,
    VCCSdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    VCCSask,
    NULL,
    VCCSpzLoad,
    NULL,
#ifndef NOSENS
    VCCSsSetup,
    VCCSsLoad,
    NULL,
    VCCSsAcLoad,
    VCCSsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(VCCSinstance),
    sizeof(VCCSmodel),


};

