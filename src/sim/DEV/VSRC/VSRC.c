/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "IFsim.h"
#include "VSRCdefs.h"
#include "suffix.h"

RCSID("VSRC.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:25 $")

static IFparm VSRCpTable[] = { /* parameters */ 
 IOP("dc",      VSRC_DC,        IF_REAL   ,"D.C. source value"),
 IOP("acmag",   VSRC_AC_MAG,    IF_REAL   ,"A.C. Magnitude"),
 IOP("acphase", VSRC_AC_PHASE,  IF_REAL   ,"A.C. Phase"),
 IP ("pulse",   VSRC_PULSE,     IF_REALVEC,"Pulse description"),
 IP ("sine",    VSRC_SINE,      IF_REALVEC,"Sinusoidal source description"),
 IP ("sin",     VSRC_SINE,      IF_REALVEC,"Sinusoidal source description"),
 IP ("exp",     VSRC_EXP,       IF_REALVEC,"Exponential source description"),
 IP ("pwl",     VSRC_PWL,       IF_REALVEC,"Piecewise linear description"),
 IP ("sffm",    VSRC_SFFM,      IF_REALVEC,"Single freq. FM descripton"),
 OP ("pos_node",VSRC_POS_NODE,  IF_INTEGER,"Positive node of source"),
 OP ("neg_node",VSRC_NEG_NODE,  IF_INTEGER,"Negative node of source"),
 OP ("function",VSRC_FCN_TYPE,  IF_INTEGER,"Function of the source"),
 OP ("order",   VSRC_FCN_ORDER, IF_INTEGER,"Order of the source function"),
 OP ("coeffs",  VSRC_FCN_COEFFS,IF_REALVEC,"Coefficients for the function"),
 OP ("acreal",  VSRC_AC_REAL,   IF_REAL,   "AC real part"),
 OP ("acimag",  VSRC_AC_IMAG,   IF_REAL,   "AC imaginary part"),
 IP ("ac",      VSRC_AC,        IF_REALVEC,"AC magnitude, phase vector"),
 OP ("c",       VSRC_CURRENT,   IF_REAL,   "Voltage source current"),
 OP ("p",       VSRC_POWER,     IF_REAL,   "Instantaneous power")
};

static char *VSRCnames[] = {
    "V+",
    "V-"
};

/*static IFparm VSRCmPTable[] = { /* model parameters */ 
/*}; /**/

SPICEdev VSRCinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Vsource", 
        "Independent voltage source",

        sizeof(VSRCnames)/sizeof(char *),
        sizeof(VSRCnames)/sizeof(char *),
        VSRCnames,

        sizeof(VSRCpTable)/sizeof(IFparm),
        VSRCpTable,

        0/*sizeof(VSRCmPTable)/sizeof(IFparm)/**/,
        NULL/*VSRCmPTable/**/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    VSRCparam,
    NULL,
    VSRCload,
    VSRCsetup,
    VSRCpzSetup,
    VSRCtemp,
    NULL,
    VSRCfindBr,
    VSRCacLoad,
    VSRCaccept,
    VSRCdestroy,
#ifdef DELETES
    VSRCmDelete,
    VSRCdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    VSRCask,
    NULL,
    VSRCpzLoad,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(VSRCinstance),
    sizeof(VSRCmodel),
};

