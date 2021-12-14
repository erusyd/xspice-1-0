/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "URCdefs.h"
#include "suffix.h"

RCSID("URC.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:38 $")

static IFparm URCpTable[] = { /* parameters */ 
 IOP( "l",      URC_LEN,   IF_REAL, "Length of transmission line"),
 IOP( "n",      URC_LUMPS, IF_REAL, "Number of lumps"),
 OP( "pos_node",URC_POS_NODE,IF_INTEGER,"Positive node of URC"),
 OP( "neg_node",URC_NEG_NODE,IF_INTEGER,"Negative node of URC"),
 OP( "gnd",     URC_GND_NODE,IF_INTEGER,"Ground node of URC")
};

static IFparm URCmPTable[] = { /* model parameters */
 IOP( "k",      URC_MOD_K,      IF_REAL, "Propagation constant"),
 IOP( "fmax",   URC_MOD_FMAX,   IF_REAL, "Maximum frequency of interest"),
 IOP( "rperl",  URC_MOD_RPERL,  IF_REAL, "Resistance per unit length"),
 IOP( "cperl",  URC_MOD_CPERL,  IF_REAL, "Capacitance per unit length"),
 IOP( "isperl", URC_MOD_ISPERL, IF_REAL, "Saturation current per length"),
 IOP( "rsperl", URC_MOD_RSPERL, IF_REAL, "Diode resistance per length"),
 IP( "urc",    URC_MOD_URC,    IF_FLAG, "Uniform R.C. line model")
};

static char *URCnames[] = {
    "P1",
    "P2",
    "Ref"
};

SPICEdev URCinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "URC",      /* MUST precede both resistors and capacitors */
        "Uniform R.C. line",

        sizeof(URCnames)/sizeof(char *),
        sizeof(URCnames)/sizeof(char *),
        URCnames,

        sizeof(URCpTable)/sizeof(IFparm),
        URCpTable,

        sizeof(URCmPTable)/sizeof(IFparm),
        URCmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    URCparam,
    URCmParam,
    NULL,
    URCsetup,
    URCsetup,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    URCdestroy,
#ifdef DELETES
    URCmDelete,
    URCdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    URCask,
    URCmAsk,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(URCinstance),
    sizeof(URCmodel),

};

