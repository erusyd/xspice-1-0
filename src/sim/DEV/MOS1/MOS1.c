/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "IFsim.h"
#include "MOS1defs.h"
#include "suffix.h"

RCSID("MOS1.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:19 $")

static IFparm MOS1pTable[] = { /* parameters */ 
 IOP("l",            MOS1_L,          IF_REAL   , "Length"),
 IOP("w",            MOS1_W,          IF_REAL   , "Width"),
 IOP("ad",           MOS1_AD,         IF_REAL   , "Drain area"),
 IOP("as",           MOS1_AS,         IF_REAL   , "Source area"),
 IOP("pd",           MOS1_PD,         IF_REAL   , "Drain perimeter"),
 IOP("ps",           MOS1_PS,         IF_REAL   , "Source perimeter"),
 IOP("nrd",          MOS1_NRD,        IF_REAL   , "Drain squares"),
 IOP("nrs",          MOS1_NRS,        IF_REAL   , "Source squares"),
 IP("off",           MOS1_OFF,        IF_FLAG   , "Device initially off"),
 IOP("icvds",        MOS1_IC_VDS,     IF_REAL   , "Initial D-S voltage"),
 IOP("icvgs",        MOS1_IC_VGS,     IF_REAL   , "Initial G-S voltage"),
 IOP("icvbs",        MOS1_IC_VBS,     IF_REAL   , "Initial B-S voltage"),
 IOP("temp",         MOS1_TEMP,       IF_REAL,    "Instance temperature"),
 IP( "ic",           MOS1_IC,  IF_REALVEC, "Vector of D-S, G-S, B-S voltages"),
 IP( "sens_l", MOS1_L_SENS, IF_FLAG, "flag to request sensitivity WRT length"),
 IP( "sens_w", MOS1_W_SENS, IF_FLAG, "flag to request sensitivity WRT width"),
 OP( "cgs",          MOS1_CGS,        IF_REAL   , "Gate-Source capacitance"),
 OP( "cgd",          MOS1_CGD,        IF_REAL   , "Gate-Drain capacitance"),
 OP( "dnode",        MOS1_DNODE,      IF_INTEGER, "Number of the drain node "),
 OP( "gnode",        MOS1_GNODE,      IF_INTEGER, "Number of the gate node "),
 OP( "snode",        MOS1_SNODE,      IF_INTEGER, "Number of the source node "),
 OP( "bnode",        MOS1_BNODE,      IF_INTEGER, "Number of the node "),
 OP( "dnodeprime", MOS1_DNODEPRIME, IF_INTEGER, "Number of int. drain node"),
 OP( "snodeprime", MOS1_SNODEPRIME, IF_INTEGER, "Number of int. source node "),
 OP( "sourceconductance", MOS1_SOURCECONDUCT, IF_REAL, "Conductance of source"),
 OP( "drainconductance",  MOS1_DRAINCONDUCT,  IF_REAL, "Conductance of drain"),
 OP( "von",          MOS1_VON,        IF_REAL,    " "),
 OP( "vdsat",        MOS1_VDSAT,      IF_REAL,    "Saturation drain voltage"),
 OP( "sourcevcrit",  MOS1_SOURCEVCRIT,IF_REAL,    "Critical source voltage"),
 OP( "drainvcrit",   MOS1_DRAINVCRIT, IF_REAL,    "Critical drain voltage"),
 OP( "cd",           MOS1_CD,         IF_REAL,    "Drain current"),
 OP( "cbs",      MOS1_CBS,    IF_REAL,    "B-S junction capacitance"),
 OP( "cbd",      MOS1_CBD,    IF_REAL,    "B-D junction capacitance"),
 OP( "gmbs",     MOS1_GMBS,   IF_REAL,    "Bulk-Source transconductance"),
 OP( "gm",           MOS1_GM,         IF_REAL,    "Transconductance"),
 OP( "gds",          MOS1_GDS,        IF_REAL,    "Drain-Source conductance"),
 OP( "gbd",          MOS1_GBD,        IF_REAL,    "Bulk-Drain conductance"),
 OP( "gbs",          MOS1_GBS,        IF_REAL,    "Bulk-Source conductance"),
 OP( "capbd",        MOS1_CAPBD,      IF_REAL,    "Bulk-Drain capacitance"),
 OP( "capbs",        MOS1_CAPBS,      IF_REAL,    "Bulk-Source capacitance"),
 OP( "cbd0", MOS1_CAPZEROBIASBD, IF_REAL, "Zero-Bias B-D junction capacitance"),
 OP( "cbdsw0",        MOS1_CAPZEROBIASBDSW, IF_REAL,    " "),
 OP( "cbs0", MOS1_CAPZEROBIASBS, IF_REAL, "Zero-Bias B-S junction capacitance"),
 OP( "cbssw0",        MOS1_CAPZEROBIASBSSW, IF_REAL,    " "),
 OP( "vbd",          MOS1_VBD,        IF_REAL,    "Bulk-Drain voltage"),
 OP( "vbs",          MOS1_VBS,        IF_REAL,    "Bulk-Source voltage"),
 OP( "vgs",          MOS1_VGS,        IF_REAL,    "Gate-Source voltage"),
 OP( "vds",          MOS1_VDS,        IF_REAL,    "Drain-Source voltage"),
 OP( "capgs",        MOS1_CAPGS,      IF_REAL,    "Gate-Source capacitance"),
 OP( "qgs",          MOS1_QGS,        IF_REAL,    "Gate-Source charge storage"),
 OP( "cqgs",MOS1_CQGS,IF_REAL,"Capacitance due to gate-source charge storage"),
 OP( "capgd",        MOS1_CAPGD,      IF_REAL,    "Gate-Drain capacitance"),
 OP( "qgd",          MOS1_QGD,        IF_REAL,    "Gate-Drain charge storage"),
 OP( "cqgd",MOS1_CQGD,IF_REAL,"Capacitance due to gate-drain charge storage"),
 OP( "capgb",        MOS1_CAPGB,      IF_REAL,    "Gate-Bulk capacitance"),
 OP( "qgb",          MOS1_QGB,        IF_REAL,    "Gate-Bulk charge storage"),
 OP( "cqgb",MOS1_CQGB,IF_REAL,"Capacitance due to gate-bulk charge storage"),
 OP( "qbd",          MOS1_QBD,        IF_REAL,    "Bulk-Drain charge storage"),
 OP( "cqbd",MOS1_CQBD,IF_REAL,"Capacitance due to bulk-drain charge storage"),
 OP( "qbs",          MOS1_QBS,        IF_REAL,    "Bulk-Source charge storage"),
 OP( "cqbs",MOS1_CQBS,IF_REAL,"Capacitance due to bulk-source charge storage"),
 OP( "cs",           MOS1_CS,         IF_REAL,    "Source current"),
 OP( "cg",           MOS1_CG,         IF_REAL,    "Gate current "),
 OP( "cb",           MOS1_CB,         IF_REAL,    "Bulk current "),
 OP( "p",            MOS1_POWER,      IF_REAL,    "Instaneous power"),
 OP( "sens_l_dc",    MOS1_L_SENS_DC,  IF_REAL,    "dc sensitivity wrt length"),
 OP( "sens_l_real", MOS1_L_SENS_REAL,IF_REAL,
        "real part of ac sensitivity wrt length"),
 OP( "sens_l_imag",  MOS1_L_SENS_IMAG,IF_REAL,    
        "imag part of ac sensitivity wrt length"),
 OP( "sens_l_mag",   MOS1_L_SENS_MAG, IF_REAL,    
        "sensitivity wrt l of ac magnitude"),
 OP( "sens_l_ph",    MOS1_L_SENS_PH,  IF_REAL,    
        "sensitivity wrt l of ac phase"),
 OP( "sens_l_cplx",  MOS1_L_SENS_CPLX,IF_COMPLEX, "ac sensitivity wrt length"),
 OP( "sens_w_dc",    MOS1_W_SENS_DC,  IF_REAL,    "dc sensitivity wrt width"),
 OP( "sens_w_real",  MOS1_W_SENS_REAL,IF_REAL,    
        "real part of ac sensitivity wrt width"),
 OP( "sens_w_imag",  MOS1_W_SENS_IMAG,IF_REAL,    
        "imag part of ac sensitivity wrt width"),
 OP( "sens_w_mag",   MOS1_W_SENS_MAG, IF_REAL,    
        "sensitivity wrt w of ac magnitude"),
 OP( "sens_w_ph",    MOS1_W_SENS_PH,  IF_REAL,    
        "sensitivity wrt w of ac phase"),
 OP( "sens_w_cplx",  MOS1_W_SENS_CPLX,IF_COMPLEX, "ac sensitivity wrt width")
};

static IFparm MOS1mPTable[] = { /* model parameters */
 IOP("vto",   MOS1_MOD_VTO,   IF_REAL   ,"Threshold voltage"),
 IOP("vt0",   MOS1_MOD_VTO,   IF_REAL   ,"Threshold voltage"),
 IOP("kp",    MOS1_MOD_KP,    IF_REAL   ,"Transconductance parameter"),
 IOP("gamma", MOS1_MOD_GAMMA, IF_REAL   ,"Bulk threshold parameter"),
 IOP("phi",   MOS1_MOD_PHI,   IF_REAL   ,"Surface potential"),
 IOP("lambda",MOS1_MOD_LAMBDA,IF_REAL   ,"Channel length modulation"),
 IOP("rd",    MOS1_MOD_RD,    IF_REAL   ,"Drain ohmic resistance"),
 IOP("rs",    MOS1_MOD_RS,    IF_REAL   ,"Source ohmic resistance"),
 IOP("cbd",   MOS1_MOD_CBD,   IF_REAL   ,"B-D junction capacitance"),
 IOP("cbs",   MOS1_MOD_CBS,   IF_REAL   ,"B-S junction capacitance"),
 IOP("is",    MOS1_MOD_IS,    IF_REAL   ,"Bulk junction sat. current"),
 IOP("pb",    MOS1_MOD_PB,    IF_REAL   ,"Bulk junction potential"),
 IOP("cgso",  MOS1_MOD_CGSO,  IF_REAL   ,"Gate-source overlap cap."),
 IOP("cgdo",  MOS1_MOD_CGDO,  IF_REAL   ,"Gate-drain overlap cap."),
 IOP("cgbo",  MOS1_MOD_CGBO,  IF_REAL   ,"Gate-bulk overlap cap."),
 IOP("rsh",   MOS1_MOD_RSH,   IF_REAL   ,"Sheet resistance"),
 IOP("cj",    MOS1_MOD_CJ,    IF_REAL   ,"Bottom junction cap per area"),
 IOP("mj",    MOS1_MOD_MJ,    IF_REAL   ,"Bottom grading coefficient"),
 IOP("cjsw",  MOS1_MOD_CJSW,  IF_REAL   ,"Side junction cap per area"),
 IOP("mjsw",  MOS1_MOD_MJSW,  IF_REAL   ,"Side grading coefficient"),
 IOP("js",    MOS1_MOD_JS,    IF_REAL   ,"Bulk jct. sat. current density"),
 IOP("tox",   MOS1_MOD_TOX,   IF_REAL   ,"Oxide thickness"),
 IOP("ld",    MOS1_MOD_LD,    IF_REAL   ,"Lateral diffusion"),
 IOP("u0",    MOS1_MOD_U0,    IF_REAL   ,"Surface mobility"),
 IOP("uo",    MOS1_MOD_U0,    IF_REAL   ,"Surface mobility"),
 IOP("fc",    MOS1_MOD_FC,    IF_REAL   ,"Forward bias jct. fit parm."),
 IP("nmos",   MOS1_MOD_NMOS,  IF_FLAG   ,"N type MOSfet model"),
 IP("pmos",   MOS1_MOD_PMOS,  IF_FLAG   ,"P type MOSfet model"),
 IOP("nsub",  MOS1_MOD_NSUB,  IF_REAL   ,"Substrate doping"),
 IOP("tpg",   MOS1_MOD_TPG,   IF_INTEGER,"Gate type"),
 IOP("nss",   MOS1_MOD_NSS,   IF_REAL   ,"Surface state density"),
 IOP("tnom",  MOS1_MOD_TNOM,  IF_REAL   ,"Parameter measurement temperature"),
};

static char *MOS1names[] = {
    "Drain",
    "Gate",
    "Source",
    "Bulk"
};

SPICEdev MOS1info = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Mos1",
        "Level 1 MOSfet model with Meyer capacitance model",

        sizeof(MOS1names)/sizeof(char *),
        sizeof(MOS1names)/sizeof(char *),
        MOS1names,

        sizeof(MOS1pTable)/sizeof(IFparm),
        MOS1pTable,

        sizeof(MOS1mPTable)/sizeof(IFparm),
        MOS1mPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    MOS1param,
    MOS1mParam,
    MOS1load,
    MOS1setup,
    MOS1setup,
    MOS1temp,
    MOS1trunc,
    NULL,
    MOS1acLoad,
    NULL,
    MOS1destroy,
#ifdef DELETES
    MOS1mDelete,
    MOS1delete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    MOS1getic,
    MOS1ask,
    NULL,
    MOS1pzLoad,
#ifdef NEWCONV
    MOS1convTest,
#else /* NEWCONV */
    NULL,
#endif /* NEWCONV */

#ifndef NOSENS
    MOS1sSetup,
    MOS1sLoad,
    MOS1sUpdate,
    MOS1sAcLoad,
    MOS1sPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(MOS1instance),
    sizeof(MOS1model),
};

