/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "DEVdefs.h"
#include "ISRCdefs.h"
#include "suffix.h"

RCSID("ISRC.c $Revision: 1.2 $ on $Date: 91/11/08 16:21:42 $")

static IFparm ISRCpTable[] = { /* parameters */ 
 IOP( "dc",      ISRC_DC,        IF_REAL   ,"DC value of source"),
 IOP( "acmag",   ISRC_AC_MAG,    IF_REAL   ,"AC magnitude"),
 IOP( "acphase", ISRC_AC_PHASE,  IF_REAL   ,"AC phase"),
 IP ( "pulse",   ISRC_PULSE,     IF_REALVEC,"Pulse description"),
 IP ( "sine",    ISRC_SINE,      IF_REALVEC,"Sinusoidal source description"),
 IP ( "sin",     ISRC_SINE,      IF_REALVEC,"Sinusoidal source description"),
 IP ( "exp",     ISRC_EXP,       IF_REALVEC,"Exponential source description"),
 IP ( "pwl",     ISRC_PWL,       IF_REALVEC,"Piecewise linear description"),
 IP ( "sffm",    ISRC_SFFM,      IF_REALVEC,"single freq. FM description"),
 OP ( "neg_node",ISRC_NEG_NODE,  IF_INTEGER,"Negative node of source"),
 OP ( "pos_node",ISRC_POS_NODE,  IF_INTEGER,"Positive node of source"),
 OP ( "acreal",  ISRC_AC_REAL,   IF_REAL   ,"AC real part"),
 OP ( "acimag",  ISRC_AC_IMAG,   IF_REAL   ,"AC imaginary part"),
 OP ( "function",ISRC_FCN_TYPE,  IF_INTEGER,"Function of the source"),
 OP ( "order",   ISRC_FCN_ORDER, IF_INTEGER,"Order of the source function"),
 OP ( "coeffs",  ISRC_FCN_COEFFS,IF_REALVEC,"Coefficients of the source"),
 IP ( "ac",      ISRC_AC,        IF_REALVEC,"AC magnitude,phase vector"),
 IP ( "p",       ISRC_POWER,     IF_REAL,   "Power supplied by the source"),
 IP ( "c",       ISRC_DC,        IF_REAL,   "Current through current source"),
/* gtri - begin - add parameter for current source value */
 OP ( "current", ISRC_CURRENT,   IF_REAL,   "Current in DC or Transient mode")
/* gtri - end - add parameter for current source value */
};

/*static IFparm ISRCmPTable[] = { /* model parameters */ 
/*}; /**/

static char *ISRCnames[] = {
    "I+",
    "I-"
};

SPICEdev ISRCinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Isource",  
        "Independent current source",

        sizeof(ISRCnames)/sizeof(char *),
        sizeof(ISRCnames)/sizeof(char *),
        ISRCnames,

        sizeof(ISRCpTable)/sizeof(IFparm),
        ISRCpTable,

        0/*sizeof(ISRCmPTable)/sizeof(IFparm)/**/,
        NULL/*ISRCmPTable/**/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    ISRCparam,
    NULL,
    ISRCload,
    NULL,
    NULL,
    ISRCtemp,
    NULL,
    NULL,
    ISRCacLoad,
    ISRCaccept,
    ISRCdestroy,
#ifdef DELETES
    ISRCmDelete,
    ISRCdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    ISRCask,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(ISRCinstance),
    sizeof(ISRCmodel),
};

