
/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "BJTdefs.h"
#include "suffix.h"

RCSID("BJT.c $Revision: 1.4 $ on $Date: 90/11/09 16:33:46 $")

/*
 * This file defines the BJT data structures that are
 * available to the next level(s) up the calling hierarchy
 */

static IFparm BJTpTable[] = { /* parameters */
 IOP("off",      BJT_OFF,            IF_FLAG,    "Device initially off"),
 IOP("icvbe",      BJT_IC_VBE,         IF_REAL,    "Initial B-E voltage"),
 IOP("icvce",      BJT_IC_VCE,         IF_REAL,    "Initial C-E voltage"),
 IOP("area",     BJT_AREA,           IF_REAL,    "Area factor"),
 IP("ic",       BJT_IC,             IF_REALVEC, "Initial condition vector"),
 IP("sens_area",BJT_AREA_SENS,IF_FLAG, "flag to request sensitivity WRT area"),
 OP("colnode",  BJT_QUEST_COLNODE,  IF_INTEGER, "Number of collector node"),
 OP("basenode", BJT_QUEST_BASENODE, IF_INTEGER, "Number of base node"),
 OP("emitnode", BJT_QUEST_EMITNODE, IF_INTEGER, "Number of emitter node"),
 OP("substnode",BJT_QUEST_SUBSTNODE,IF_INTEGER, "Number of substrate node"),
 OP("colprimenode",BJT_QUEST_COLPRIMENODE,IF_INTEGER,"Internal collector node"),
 OP("baseprimenode",BJT_QUEST_BASEPRIMENODE,IF_INTEGER,"Internal base node"),
 OP("emitprimenode",BJT_QUEST_EMITPRIMENODE,IF_INTEGER,"Internal emitter node"),
 OP("vbe",   BJT_QUEST_VBE,  IF_REAL, "B-E voltage"),
 OP("vbc",   BJT_QUEST_VBC,  IF_REAL, "B-C voltage"),
 OP("cc",    BJT_QUEST_CC,   IF_REAL, "Current at collector node"),
 OP("cb",    BJT_QUEST_CB,   IF_REAL, "Current at base node"),
 OP("gpi",   BJT_QUEST_GPI,  IF_REAL, "Small signal input conductance - pi"),
 OP("gmu",   BJT_QUEST_GMU,  IF_REAL, "Small signal conductance - mu"),
 OP("gm",    BJT_QUEST_GM,   IF_REAL, "Small signal transconductance"),
 OP("go",    BJT_QUEST_GO,   IF_REAL, "Small signal output conductance"),
 OP("qbe",   BJT_QUEST_QBE,  IF_REAL, "Charge storage B-E junction"),
 OP("cqbe",  BJT_QUEST_CQBE, IF_REAL, "Cap. due to charge storage in B-E jct."),
 OP("qbc",   BJT_QUEST_QBC,  IF_REAL, "Charge storage B-C junction"),
 OP("cqbc",  BJT_QUEST_CQBC, IF_REAL, "Cap. due to charge storage in B-C jct."),
 OP("qcs",  BJT_QUEST_QCS,  IF_REAL, "Charge storage C-S junction"),
 OP("cqcs", BJT_QUEST_CQCS, IF_REAL, "Cap. due to charge storage in C-S jct."),
 OP("qbx",  BJT_QUEST_QBX,  IF_REAL, "Charge storage B-X junction"),
 OP("cqbx", BJT_QUEST_CQBX, IF_REAL, "Cap. due to charge storage in B-X jct."),
 OP("gx",   BJT_QUEST_GX,   IF_REAL, "Conductance from base to internal base"),
 OP("cexbc",BJT_QUEST_CEXBC,IF_REAL, "Total Capacitance in B-X junction"),
 OP("geqcb",BJT_QUEST_GEQCB,IF_REAL, "d(Ibe)/d(Vbc)"),
 OP("gccs", BJT_QUEST_GCCS, IF_REAL, "Internal C-S cap. equiv. cond."),
 OP("geqbx",BJT_QUEST_GEQBX,IF_REAL, "Internal C-B-base cap. equiv. cond."),
 OP("ce",   BJT_QUEST_CE,   IF_REAL, "Emitter current"),
 OP("cs",   BJT_QUEST_CS,   IF_REAL, "Substrate current"),
 OP("p",    BJT_QUEST_POWER,IF_REAL, "Power dissipation"),
 OP("sens_dc", BJT_QUEST_SENS_DC, IF_REAL,    "dc sensitivity "),
 OP("sens_real", BJT_QUEST_SENS_REAL, IF_REAL,"real part of ac sensitivity"),
 OP("sens_imag",BJT_QUEST_SENS_IMAG,IF_REAL,"dc sens. & imag part of ac sens."),
 OP("sens_mag",  BJT_QUEST_SENS_MAG,  IF_REAL,   "sensitivity of ac magnitude"),
 OP("sens_ph",   BJT_QUEST_SENS_PH,   IF_REAL,    "sensitivity of ac phase"),
 OP("sens_cplx", BJT_QUEST_SENS_CPLX, IF_COMPLEX, "ac sensitivity"),
 IOP("temp",     BJT_TEMP,            IF_REAL,    "instance temperature"),
};

static IFparm BJTmPTable[] = { /* model parameters */
 IOP("npn",  BJT_MOD_NPN,  IF_FLAG, "NPN type device"),
 IOP("pnp",  BJT_MOD_PNP,  IF_FLAG, "PNP type device"),
 IOP("is",   BJT_MOD_IS,   IF_REAL, "Saturation Current"),
 IOP("bf",   BJT_MOD_BF,   IF_REAL, "Ideal forward beta"),
 IOP("nf",   BJT_MOD_NF,   IF_REAL, "Forward emission coefficient"),
 IOP("vaf",  BJT_MOD_VAF,  IF_REAL, "Forward Early voltage"),
 IOP("va",   BJT_MOD_VAF,  IF_REAL, "Forward Early voltage"),
 IOP("ikf",  BJT_MOD_IKF,  IF_REAL, "Forward beta roll-off corner current"),
 IOP("ik",   BJT_MOD_IKF,  IF_REAL, "Forward beta roll-off corner current"),
 IOP("ise",  BJT_MOD_ISE,  IF_REAL, "B-E leakage saturation current"),
 IOP("c2",   BJT_MOD_C2,   IF_REAL, "Obsolete parameter name"),
 IOP("ne",   BJT_MOD_NE,   IF_REAL, "B-E leakage emission coefficient"),
 IOP("br",   BJT_MOD_BR,   IF_REAL, "Ideal reverse beta"),
 IOP("nr",   BJT_MOD_NR,   IF_REAL, "Reverse emission coefficient"),
 IOP("var",  BJT_MOD_VAR,  IF_REAL, "Reverse Early voltage"),
 IOP("vb",   BJT_MOD_VAR,  IF_REAL, "Reverse Early voltage"),
 IOP("ikr",  BJT_MOD_IKR,  IF_REAL, "reverse beta roll-off corner current"),
 IOP("isc",  BJT_MOD_ISC,  IF_REAL, "B-C leakage saturation current"),
 IOP("c4",   BJT_MOD_C4,   IF_REAL, "Obsolete parameter name"),
 IOP("nc",   BJT_MOD_NC,   IF_REAL, "B-C leakage emission coefficient"),
 IOP("rb",   BJT_MOD_RB,   IF_REAL, "Zero bias base resistance"),
 IOP("irb",  BJT_MOD_IRB,  IF_REAL, "Current for base resistance=(rb+rbm)/2"),
 IOP("rbm",  BJT_MOD_RBM,  IF_REAL, "Minimum base resistance"),
 IOP("re",   BJT_MOD_RE,   IF_REAL, "Emitter resistance"),
 IOP("rc",   BJT_MOD_RC,   IF_REAL, "Collector resistance"),
 IOP("cje",  BJT_MOD_CJE,  IF_REAL, "Zero bias B-E depletion capacitance"),
 IOP("vje",  BJT_MOD_VJE,  IF_REAL, "B-E built in potential"),
 IOP("pe",   BJT_MOD_VJE,  IF_REAL, "B-E built in potential"),
 IOP("mje",  BJT_MOD_MJE,  IF_REAL, "B-E junction grading coefficient"),
 IOP("me",   BJT_MOD_MJE,  IF_REAL, "B-E junction grading coefficient"),
 IOP("tf",   BJT_MOD_TF,   IF_REAL, "Ideal forward transit time"),
 IOP("xtf",  BJT_MOD_XTF,  IF_REAL, "Coefficient for bias dependence of TF"),
 IOP("vtf",  BJT_MOD_VTF,  IF_REAL, "Voltage giving VBC dependence of TF"),
 IOP("itf",  BJT_MOD_ITF,  IF_REAL, "High current dependence of TF"),
 IOP("ptf",  BJT_MOD_PTF,  IF_REAL, "Excess phase"),
 IOP("cjc",  BJT_MOD_CJC,  IF_REAL, "Zero bias B-C depletion capacitance"),
 IOP("vjc",  BJT_MOD_VJC,  IF_REAL, "B-C built in potential"),
 IOP("pc",   BJT_MOD_VJC,  IF_REAL, "B-C built in potential"),
 IOP("mjc",  BJT_MOD_MJC,  IF_REAL, "B-C junction grading coefficient"),
 IOP("mc",   BJT_MOD_MJC,  IF_REAL, "B-C junction grading coefficient"),
 IOP("xcjc", BJT_MOD_XCJC, IF_REAL, "Fraction of B-C cap to internal base"),
 IOP("tr",   BJT_MOD_TR,   IF_REAL, "Ideal reverse transit time"),
 IOP("cjs",  BJT_MOD_CJS,  IF_REAL, "Zero bias C-S capacitance"),
 IOP("ccs",  BJT_MOD_CJS,  IF_REAL, "Zero bias C-S capacitance"),
 IOP("vjs",  BJT_MOD_VJS,  IF_REAL, "Substrate junction built in potential"),
 IOP("ps",   BJT_MOD_VJS,  IF_REAL, "Substrate junction built in potential"),
 IOP("mjs",  BJT_MOD_MJS,  IF_REAL, "Substrate junction grading coefficient"),
 IOP("ms",   BJT_MOD_MJS,  IF_REAL, "Substrate junction grading coefficient"),
 IOP("xtb",  BJT_MOD_XTB,  IF_REAL, "Forward and reverse beta temp. exp."),
 IOP("eg",   BJT_MOD_EG,   IF_REAL, "Energy gap for IS temp. dependency"),
 IOP("xti",  BJT_MOD_XTI,  IF_REAL, "Temp. exponent for IS"),
 IOP("fc",   BJT_MOD_FC,   IF_REAL, "Forward bias junction fit parameter"),
 OP("invearlyvoltf",BJT_MOD_INVEARLYF,IF_REAL,"Inverse early voltage:forward"),
 OP("invearlyvoltr",BJT_MOD_INVEARLYR,IF_REAL,"Inverse early voltage:reverse"),
 OP("invrollofff",  BJT_MOD_INVROLLOFFF,  IF_REAL,"Inverse roll off - forward"),
 OP("invrolloffr",  BJT_MOD_INVROLLOFFR,  IF_REAL,"Inverse roll off - reverse"),
 OP("collectorconduct",BJT_MOD_COLCONDUCT,IF_REAL,"Collector conductance"),
 OP("emitterconduct", BJT_MOD_EMITTERCONDUCT,IF_REAL, "Emitter conductance"),
 OP("transtimevbcfact",BJT_MOD_TRANSVBCFACT,IF_REAL,"Transit time VBC factor"),
 OP("excessphasefactor",BJT_MOD_EXCESSPHASEFACTOR,IF_REAL,"Excess phase fact."),
 IOP("tnom", BJT_MOD_TNOM, IF_REAL, "Parameter measurement temperature"),
};

static char *BJTnames[] = {
    "collector",
    "base",
    "emitter",
    "substrate"
};


SPICEdev BJTinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {   "BJT",
        "Bipolar Junction Transistor",

        sizeof(BJTnames)/sizeof(char *),
        sizeof(BJTnames)/sizeof(char *),
        BJTnames,

        sizeof(BJTpTable)/sizeof(IFparm),
        BJTpTable,

        sizeof(BJTmPTable)/sizeof(IFparm),
        BJTmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    BJTparam,
    BJTmParam,
    BJTload,
    BJTsetup,
    BJTsetup,
    BJTtemp,
    BJTtrunc,
    NULL,
    BJTacLoad,
    NULL,
    BJTdestroy,
#ifdef DELETES
    BJTmDelete,
    BJTdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    BJTgetic,
    BJTask,
    NULL,
    BJTpzLoad,
#ifdef NEWCONV
    BJTconvTest,
#else /* NEWCONV */
    NULL,
#endif /* NEWCONV */

#ifndef NOSENS
    BJTsSetup,
    BJTsLoad,
    BJTsUpdate,
    BJTsAcLoad,
    BJTsPrint,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */
    NULL,

    sizeof(BJTinstance),
    sizeof(BJTmodel),

};

