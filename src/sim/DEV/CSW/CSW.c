/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "DEVdefs.h"
#include "CSWdefs.h"
#include "suffix.h"

RCSID("CSW.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:01 $")

static IFparm CSWpTable[] = { /* parameters */ 
 IOP( "control",CSW_CONTROL, IF_INSTANCE, "Name of controlling source"),
 IP( "on",  CSW_IC_ON,   IF_FLAG    , "Initially closed"),
 IP( "off",     CSW_IC_OFF,  IF_FLAG    , "Initially open"),
 OP( "pos_node",CSW_POS_NODE,IF_INTEGER,  "Positive node of switch"),
 OP( "neg_node",CSW_NEG_NODE,IF_INTEGER,  "Negative node of switch"),
 OP( "c"       ,CSW_CURRENT, IF_REAL,     "Switch current"),
 OP( "p"       ,CSW_POWER,   IF_REAL,     "Instantaneous power")
};

static IFparm CSWmPtable[] = { /* model parameters */
 IOP( "csw",  CSW_CSW,  IF_FLAG, "Current controlled switch model"),
 IOP( "it",   CSW_ITH,  IF_REAL, "Threshold current"),
 IOP( "ih",   CSW_IHYS, IF_REAL, "Hysterisis current"),
 IOP( "ron",  CSW_RON,  IF_REAL, "Closed resistance"),
 IOP( "roff", CSW_ROFF, IF_REAL, "Open resistance"),
 OP( "gon",   CSW_GON,  IF_REAL, "Closed conductance"),
 OP( "goff",  CSW_GOFF, IF_REAL, "Open conductance")
};

static char *CSWnames[] = {
    "W+",
    "W-"
};

SPICEdev CSWinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {   "CSwitch",
        "Current controlled ideal switch",

        sizeof(CSWnames)/sizeof(char *),
        sizeof(CSWnames)/sizeof(char *),
        CSWnames,

        sizeof(CSWpTable)/sizeof(IFparm),
        CSWpTable,

        sizeof(CSWmPtable)/sizeof(IFparm),
        CSWmPtable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    CSWparam,
    CSWmParam,
    CSWload,
    CSWsetup,
    CSWsetup,
    NULL,
    NULL,
    NULL,
    CSWacLoad,
    NULL,
    CSWdestroy,
#ifdef DELETES
    CSWmDelete,
    CSWdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    CSWask,
    CSWmAsk,
    CSWpzLoad,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(CSWinstance),
    sizeof(CSWmodel),

};

