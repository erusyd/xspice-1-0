/*
 * Copyright (c) 1987 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "DEVdefs.h"
#include "ASRCdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("ASRC.c $Revision: 1.4 $ on $Date: 90/11/09 16:33:44 $")

/* Arbitrary source */
static IFparm ASRCpTable[] = { /* parameters */ 
 IP( "i", ASRC_CURRENT, IF_PARSETREE, "Current source "),
 IP( "v", ASRC_VOLTAGE, IF_PARSETREE, "Voltage source"),
 OP( "pos_node", ASRC_POS_NODE, IF_INTEGER, "Positive Node"),
 OP( "neg_node", ASRC_NEG_NODE, IF_INTEGER, "Negative Node")
};

/*static IFparm ASRCmPTable[] = { /* model parameters */
/*}; /**/

static char *ASRCnames[] = {
    "src+",
    "src-"
};

SPICEdev ASRCinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "ASRC",
        "Arbitrary Source ",

        sizeof(ASRCnames)/sizeof(char *),
        sizeof(ASRCnames)/sizeof(char *),
        ASRCnames,

        sizeof(ASRCpTable)/sizeof(IFparm),
        ASRCpTable,

        0/*sizeof(ASRCmPTable)/sizeof(IFparm) */,
        NULL/*ASRCmPTable*/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    ASRCparam,
    NULL,
    ASRCload,
    ASRCsetup,
    ASRCsetup,
    NULL,
    NULL,
    ASRCfindBr,
    ASRCacLoad,   /* ac and normal load functions NOT identical */
    NULL,
    ASRCdestroy,
#ifdef DELETES
    ASRCmDelete,
    ASRCdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    NULL,
    NULL,
    ASRCpzLoad,
#ifdef NEWCONV
    ASRCconvTest,
#else /* NEWCONV */
    NULL,
#endif /* NEWCONV */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(ASRCinstance),
    sizeof(ASRCmodel),
};
