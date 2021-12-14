/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "DEVdefs.h"
#include "MESdefs.h"
#include "suffix.h"

RCSID("MES.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:16 $")

static IFparm MESpTable[] = { /* parameters */ 
 OP("off",  MES_OFF,    IF_FLAG   ,"Device initially off"),
 IOP("area", MES_AREA,   IF_REAL   ,"Area factor"),
 IOP("icvds",  MES_IC_VDS, IF_REAL   ,"Initial D-S voltage"),
 IOP("icvgs",  MES_IC_VGS, IF_REAL   ,"Initial G-S voltage"),
 OP("dnode", MES_DRAINNODE, IF_INTEGER,"Number of drain node"),
 OP("gnode", MES_GATENODE, IF_INTEGER,"Number of gate node"),
 OP("snode", MES_SOURCENODE, IF_INTEGER,"Number of source node"),
 OP("dprimenode",MES_DRAINPRIMENODE,IF_INTEGER,"Number of internal drain node"),
 OP("sprimenode",MES_SOURCEPRIMENODE,IF_INTEGER,
        "Number of internal source node"),
 OP("vgs", MES_VGS, IF_REAL,"Gate-Source voltage"),
 OP("vgd", MES_VGD, IF_REAL,"Gate-Drain voltage"),
 OP("cg", MES_CG, IF_REAL,"Gate capacitance"),
 OP("cd", MES_CD, IF_REAL,"Drain capacitance"),
 OP("cgd", MES_CGD, IF_REAL,"Gate_Drain capacitance"),
 OP("gm", MES_GM, IF_REAL,"Transconductance"),
 OP("gds", MES_GDS, IF_REAL,"Drain-Source conductance"),
 OP("ggs", MES_GGS, IF_REAL,"Gate-Source conductance"),
 OP("ggd", MES_GGD, IF_REAL,"Gate-Drain conductance"),
 OP("qgs", MES_QGS, IF_REAL,"Gate-Source charge storage"),
 OP("cqgs", MES_CQGS, IF_REAL,"Capacitance due to gate-source charge storage"),
 OP("qgd", MES_QGD, IF_REAL,"Gate-Drain charge storage"),
 OP("cqgd", MES_CQGD, IF_REAL,"Capacitance due to gate-drain charge storage"),
 OP("cs",   MES_CS,     IF_REAL   ,"Source current"),
 OP("p",    MES_POWER,  IF_REAL   ,"Power dissipated by the mesfet")

};

static IFparm MESmPTable[] = { /* model parameters */
 IOP( "vt0",    MES_MOD_VTO,    IF_REAL,"Pinch-off voltage"),
 IOP( "vto",    MES_MOD_VTO,    IF_REAL,"Pinch-off voltage"),
 IOP( "alpha",  MES_MOD_ALPHA,  IF_REAL,"Saturation voltage parameter"),
 IOP( "beta",   MES_MOD_BETA,   IF_REAL,"Transconductance parameter"),
 IOP( "lambda", MES_MOD_LAMBDA, IF_REAL,"Channel length modulation parm."),
 IOP( "b",      MES_MOD_B,      IF_REAL,"Doping tail extending parameter"),
 IOP( "rd",     MES_MOD_RD,     IF_REAL,"Drain ohmic resistance"),
 IOP( "rs",     MES_MOD_RS,     IF_REAL,"Source ohmic resistance"),
 IOP( "cgs",    MES_MOD_CGS,    IF_REAL,"G-S junction capacitance"),
 IOP( "cgd",    MES_MOD_CGD,    IF_REAL,"G-D junction capacitance"),
 IOP( "pb",     MES_MOD_PB,     IF_REAL,"Gate junction potential"),
 IOP( "is",     MES_MOD_IS,     IF_REAL,"Junction saturation current"),
 IOP( "fc",     MES_MOD_FC,     IF_REAL,"Forward biad junction fit parm."),
 OP( "nmf",    MES_MOD_NMF,    IF_FLAG,"N type MESfet model"),
 OP( "pmf",    MES_MOD_PMF,    IF_FLAG,"P type MESfet model"),
 OP( "gd",  MES_MOD_DRAINCONDUCT,   IF_REAL,"Drain conductance"),
 OP( "gs",  MES_MOD_SOURCECONDUCT,  IF_REAL,"Source conductance"),
 OP( "depl_cap",    MES_MOD_DEPLETIONCAP,   IF_REAL,"Depletion capacitance"),
 OP( "vcrit",   MES_MOD_VCRIT,  IF_REAL,"Critical voltage"),
 P("af",        0,           IF_REAL,"Flicker noise exponent"),
 P("kf",        0,           IF_REAL,"Flicker noise coefficient"),
};

static char *MESnames[] = {
    "Drain",
    "Gate",
    "Source"
};

SPICEdev MESinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "MES",
        "GaAs MESFET model",

        sizeof(MESnames)/sizeof(char *),
        sizeof(MESnames)/sizeof(char *),
        MESnames,

        sizeof(MESpTable)/sizeof(IFparm),
        MESpTable,

        sizeof(MESmPTable)/sizeof(IFparm),
        MESmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    MESparam,
    MESmParam,
    MESload,
    MESsetup,
    MESsetup,
    MEStemp,
    MEStrunc,
    NULL,
    MESacLoad,
    NULL,
    MESdestroy,
#ifdef DELETES
    MESmDelete,
    MESdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    MESgetic,
    MESask,
    MESmAsk,
    MESpzLoad,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(MESinstance),
    sizeof(MESmodel),

};
