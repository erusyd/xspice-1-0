/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "IFsim.h"
#include "TRAdefs.h"
#include "suffix.h"

RCSID("TRA.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:34 $")

static IFparm TRApTable[] = { /* parameters */ 
 IOP( "z0", TRA_Z0,   IF_REAL   , "Characteristic impedance"),
 IOP( "zo", TRA_Z0,   IF_REAL   , "Characteristic impedance"),
 IOP( "f",  TRA_FREQ, IF_REAL   , "Frequency"),
 IOP( "td", TRA_TD,   IF_REAL   , "Transmission delay"),
 IOP( "nl", TRA_NL,   IF_REAL   , "Normalized length at frequency given"),
 IOP( "v1", TRA_V1,   IF_REAL   , "Initial voltage at end 1"),
 IOP( "v2", TRA_V2,   IF_REAL   , "Initial voltage at end 2"),
 IOP( "i1", TRA_I1,   IF_REAL   , "Initial current at end 1"),
 IOP( "i2", TRA_I2,   IF_REAL   , "Initial current at end 2"),
 IP("ic", TRA_IC,   IF_REALVEC,"Initial condition vector:v1,i1,v2,i2"),
 OP( "rel", TRA_RELTOL,   IF_REAL   , "Rel. rate of change of deriv. for bkpt"),
 OP( "abs", TRA_ABSTOL,   IF_REAL   , "Abs. rate of change of deriv. for bkpt"),
 OP( "pos_node1", TRA_POS_NODE1,IF_INTEGER,"Positive node of end 1 of t. line"),
 OP( "neg_node1", TRA_NEG_NODE1,IF_INTEGER,"Negative node of end 1 of t. line"),
 OP( "pos_node2", TRA_POS_NODE2,IF_INTEGER,"Positive node of end 2 of t. line"),
 OP( "neg_node2", TRA_NEG_NODE2,IF_INTEGER,"Negative node of end 2 of t. line"),
 OP( "delays",TRA_DELAY, IF_REALVEC, "Delayed values of excitation")
};

/*static IFparm TRAmPTable[] = { /* model parameters */
/*};/**/

static char *TRAnames[] = {
    "P1+",
    "P1-",
    "P2+",
    "P2-"
};

SPICEdev TRAinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Tranline",
        "Lossless transmission line",

        sizeof(TRAnames)/sizeof(char *),
        sizeof(TRAnames)/sizeof(char *),
        TRAnames,

        sizeof(TRApTable)/sizeof(IFparm),
        TRApTable,

        0/*sizeof(TRAmPTable)/sizeof(IFparm)/**/,
        NULL/*TRAmPTable/**/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    TRAparam,
    NULL,
    TRAload,
    TRAsetup,
    TRAsetup,
    TRAtemp,
    TRAtrunc,
    NULL,
    TRAacLoad,
    TRAaccept,
    TRAdestroy,
#ifdef DELETES
    TRAmDelete,
    TRAdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    TRAask,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(TRAinstance),
    sizeof(TRAmodel),

};

