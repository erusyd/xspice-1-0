/*
 * Copyright (c) 1987
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "IFsim.h"
#include "SWdefs.h"
#include "suffix.h"

RCSID("SW.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:31 $")

static IFparm SWpTable[] = { /* parameters */ 
 IP("on",           SW_IC_ON,  IF_FLAG,"Switch initially closed"),
 IP("off",          SW_IC_OFF, IF_FLAG,"Switch initially open"),
 IOP("pos_node",    SW_POS_NODE,IF_INTEGER,"Positive node of switch"),
 IOP("neg_node",    SW_NEG_NODE,IF_INTEGER,"Negative node of switch"),
 OP("cont_p_node",SW_POS_CONT_NODE,IF_INTEGER,"Positive contr. node of switch"),
 OP("cont_n_node",SW_NEG_CONT_NODE,IF_INTEGER,"Positive contr. node of switch"),
 OP("c",            SW_CURRENT, IF_REAL,    "Switch current"),
 OP("p",            SW_POWER,   IF_REAL,    "Switch power")
};
static IFparm SWmPTable[] = { /* model parameters */
 IOP( "sw",   SW_MOD_SW,   IF_FLAG,"Switch model"),
 IOP( "vt",   SW_MOD_VTH,  IF_REAL,"Threshold voltage"),
 IOP( "vh",   SW_MOD_VHYS, IF_REAL,"Hysteresis voltage"),
 IOP( "ron",  SW_MOD_RON,  IF_REAL,"Resistance when closed"),
 IOP( "roff", SW_MOD_ROFF, IF_REAL,"Resistance when open"),
 OP( "gon", SW_MOD_GON,  IF_REAL,"Conductance when closed"),
 OP( "goff", SW_MOD_GOFF, IF_REAL,"Conductance when open")
};

static char *SWnames[] = {
    "S+",
    "S-",
    "SC+",
    "SC-"
};

SPICEdev SWinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Switch",
        "Ideal voltage controlled switch",

        sizeof(SWnames)/sizeof(char *),
        sizeof(SWnames)/sizeof(char *),
        SWnames,

        sizeof(SWpTable)/sizeof(IFparm),
        SWpTable,

        sizeof(SWmPTable)/sizeof(IFparm),
        SWmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    SWparam,
    SWmParam,
    SWload,
    SWsetup,
    SWsetup,
    NULL,
    NULL,
    NULL,
    SWacLoad,   
    NULL,
    SWdestroy,
#ifdef DELETES
    SWmDelete,
    SWdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    SWask,
    SWmAsk,
    SWpzLoad,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(SWinstance),
    sizeof(SWmodel),

};
