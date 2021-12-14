/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "IFsim.h"
#include "MOS3defs.h"
#include "suffix.h"

RCSID("MOS3.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:25 $")

static IFparm MOS3pTable[] = { /* parameters */ 
 IOP("l",         MOS3_L,       IF_REAL   , "Length"),
 IOP("w",         MOS3_W,       IF_REAL   , "Width"),
 IOP("ad",        MOS3_AD,      IF_REAL   , "Drain area"),
 IOP("as",        MOS3_AS,      IF_REAL   , "Source area"),
 IOP("pd",        MOS3_PD,      IF_REAL   , "Drain perimeter"),
 IOP("ps",        MOS3_PS,      IF_REAL   , "Source perimeter"),
 IOP("nrd",       MOS3_NRD,     IF_REAL   , "Drain squares"),
 IOP("nrs",       MOS3_NRS,     IF_REAL   , "Source squares"),
 IP("off",        MOS3_OFF,     IF_FLAG   , "Device initially off"),
 IOP("icvds",       MOS3_IC_VDS,  IF_REAL   , "Initial D-S voltage"),
 IOP("icvgs",       MOS3_IC_VGS,  IF_REAL   , "Initial G-S voltage"),
 IOP("icvbs",       MOS3_IC_VBS,  IF_REAL   , "Initial B-S voltage"),
 IOP("ic",        MOS3_IC,      IF_REALVEC, "Vector of D-S, G-S, B-S voltages"),
 IOP("temp",      MOS3_TEMP,    IF_REAL   , "Instance operating temperature"),
 IP("sens_l",  MOS3_L_SENS, IF_FLAG, "flag to request sensitivity WRT length"),
 IP("sens_w",  MOS3_W_SENS, IF_FLAG, "flag to request sensitivity WRT width"),
 OP("cgs",       MOS3_CGS,     IF_REAL   , "Gate-Source capacitance"),
 OP("cgd",       MOS3_CGD,     IF_REAL   , "Gate-Drain capacitance"),
 OP("dnode",     MOS3_DNODE,   IF_INTEGER, "Number of drain node"),
 OP("gnode",     MOS3_GNODE,   IF_INTEGER, "Number of gate node"),
 OP("snode",     MOS3_SNODE,   IF_INTEGER, "Number of source node"),
 OP("bnode",     MOS3_BNODE,   IF_INTEGER, "Number of bulk node"),
 OP("dnodeprime", MOS3_DNODEPRIME,IF_INTEGER,"Number of internal drain node"),
 OP("snodeprime", MOS3_SNODEPRIME,IF_INTEGER,"Number of internal source node"),
 OP("sourceconductance", MOS3_SOURCECONDUCT, IF_REAL,  "Source conductance"),
 OP("drainconductance",  MOS3_DRAINCONDUCT,  IF_REAL,  "Drain conductance"),
 OP("von",               MOS3_VON,           IF_REAL,    " "),
 OP("vdsat",       MOS3_VDSAT,         IF_REAL, "Saturation drain voltage"),
 OP("sourcevcrit", MOS3_SOURCEVCRIT,   IF_REAL, "Critical source voltage"),
 OP("drainvcrit",  MOS3_DRAINVCRIT,    IF_REAL, "Critical drain voltage"),
 OP("cd",    MOS3_CD,            IF_REAL, "Drain current"),
 OP("cbs",   MOS3_CBS,           IF_REAL, "B-S junction capacitance"),
 OP("cbd",   MOS3_CBD,           IF_REAL, "B-D junction capacitance"),
 OP("gmbs",  MOS3_GMBS,          IF_REAL, "Bulk-Source transconductance"),
 OP("gm",    MOS3_GM,            IF_REAL, "Transconductance"),
 OP("gds",   MOS3_GDS,           IF_REAL, "Drain-Source conductance"),
 OP("gbd",   MOS3_GBD,           IF_REAL, "Bulk-Drain conductance"),
 OP("gbs",   MOS3_GBS,           IF_REAL, "Bulk-Source conductance"),
 OP("capbd", MOS3_CAPBD,         IF_REAL, "Bulk-Drain capacitance"),
 OP("capbs", MOS3_CAPBS,         IF_REAL, "Bulk-Source capacitance"),
 OP("cbd0",  MOS3_CAPZEROBIASBD, IF_REAL,"Zero-Bias B-D junction capacitance"),
 OP("cbdsw0",MOS3_CAPZEROBIASBDSW,IF_REAL, " "),
 OP("cbs0",  MOS3_CAPZEROBIASBS, IF_REAL,"Zero-Bias B-S junction capacitance"),
 OP("cbssw0",MOS3_CAPZEROBIASBSSW,IF_REAL, " "),
 OP("vbd",   MOS3_VBD,            IF_REAL, "Bulk-Drain voltage"),
 OP("vbs",   MOS3_VBS,            IF_REAL, "Bulk-Source voltage"),
 OP("vgs",   MOS3_VGS,            IF_REAL, "Gate-Source voltage"),
 OP("vds",   MOS3_VDS,            IF_REAL, "Drain-Source voltage"),
 OP("capgs", MOS3_CAPGS,          IF_REAL, "Gate-Source capacitance"),
 OP("qgs",   MOS3_QGS,            IF_REAL, "Gate-Source charge storage"),
 OP("cqgs",MOS3_CQGS,IF_REAL,"Capacitance due to gate-source charge storage"),
 OP("capgd", MOS3_CAPGD,          IF_REAL, "Gate-Drain capacitance"),
 OP("qgd",   MOS3_QGD,            IF_REAL, "Gate-Drain charge storage"),
 OP("cqgd", MOS3_CQGD, IF_REAL,"Capacitance due to gate-drain charge storage"),
 OP("capgb",MOS3_CAPGB, IF_REAL, "Gate-Bulk capacitance"),
 OP("qgb",  MOS3_QGB,   IF_REAL, "Gate-Bulk charge storage"),
 OP("cqgb", MOS3_CQGB,  IF_REAL,"Capacitance due to gate-bulk charge storage"),
 OP("qbd",  MOS3_QBD,   IF_REAL, "Bulk-Drain charge storage"),
 OP("cqbd", MOS3_CQBD,IF_REAL,"Capacitance due to bulk-drain charge storage"),
 OP("qbs",  MOS3_QBS,   IF_REAL, "Bulk-Source charge storage"),
 OP("cqbs", MOS3_CQBS,IF_REAL,"Capacitance due to bulk-source charge storage"),
 OP("cs",   MOS3_CS,    IF_REAL, "Source current"),
 OP("cg",   MOS3_CG,    IF_REAL, "Gate current"),
 OP("cb",   MOS3_CB,    IF_REAL, "Bulk current"),
 OP("p",    MOS3_POWER, IF_REAL, "Instantaneous power"),
 OP("sens_l_dc", MOS3_L_SENS_DC,    IF_REAL, "dc sensitivity wrt length"),
 OP("sens_l_real",MOS3_L_SENS_REAL, IF_REAL, 
        "real part of ac sensitivity wrt length"),
 OP("sens_l_imag",MOS3_L_SENS_IMAG, IF_REAL, 
        "imag part of ac sensitivity wrt length"),
 OP("sens_l_cplx",MOS3_L_SENS_CPLX, IF_COMPLEX, "ac sensitivity wrt length"),
 OP("sens_l_mag", MOS3_L_SENS_MAG,  IF_REAL, 
        "sensitivity wrt l of ac magnitude"),
 OP("sens_l_ph",  MOS3_L_SENS_PH,   IF_REAL, "sensitivity wrt l of ac phase"),
 OP("sens_w_dc",  MOS3_W_SENS_DC,   IF_REAL, "dc sensitivity wrt width"),
 OP("sens_w_real",MOS3_W_SENS_REAL, IF_REAL, 
        "real part of ac sensitivity wrt width"),
 OP("sens_w_imag",MOS3_W_SENS_IMAG, IF_REAL, 
        "imag part of ac sensitivity wrt width"),
 OP("sens_w_mag", MOS3_W_SENS_MAG,  IF_REAL,
        "sensitivity wrt w of ac magnitude"),
 OP("sens_w_ph",  MOS3_W_SENS_PH,   IF_REAL, "sensitivity wrt w of ac phase"),
 OP("sens_w_cplx",MOS3_W_SENS_CPLX, IF_COMPLEX, "ac sensitivity wrt width")
};

static IFparm MOS3mPTable[] = { /* model parameters */
 IOP("vto",   MOS3_MOD_VTO,   IF_REAL   ,"Threshold voltage"),
 IOP("vt0",   MOS3_MOD_VTO,   IF_REAL   ,"Threshold voltage"),
 IOP("kp",    MOS3_MOD_KP,    IF_REAL   ,"Transconductance parameter"),
 IOP("gamma", MOS3_MOD_GAMMA, IF_REAL   ,"Bulk threshold parameter"),
 IOP("phi",   MOS3_MOD_PHI,   IF_REAL   ,"Surface potential"),
 IOP("rd",    MOS3_MOD_RD,    IF_REAL   ,"Drain ohmic resistance"),
 IOP("rs",    MOS3_MOD_RS,    IF_REAL   ,"Source ohmic resistance"),
 IOP("cbd",   MOS3_MOD_CBD,   IF_REAL   ,"B-D junction capacitance"),
 IOP("cbs",   MOS3_MOD_CBS,   IF_REAL   ,"B-S junction capacitance"),
 IOP("is",    MOS3_MOD_IS,    IF_REAL   ,"Bulk junction sat. current"),
 IOP("pb",    MOS3_MOD_PB,    IF_REAL   ,"Bulk junction potential"),
 IOP("cgso",  MOS3_MOD_CGSO,  IF_REAL   ,"Gate-source overlap cap."),
 IOP("cgdo",  MOS3_MOD_CGDO,  IF_REAL   ,"Gate-drain overlap cap."),
 IOP("cgbo",  MOS3_MOD_CGBO,  IF_REAL   ,"Gate-bulk overlap cap."),
 IOP("rsh",   MOS3_MOD_RSH,   IF_REAL   ,"Sheet resistance"),
 IOP("cj",    MOS3_MOD_CJ,    IF_REAL   ,"Bottom junction cap per area"),
 IOP("mj",    MOS3_MOD_MJ,    IF_REAL   ,"Bottom grading coefficient"),
 IOP("cjsw",  MOS3_MOD_CJSW,  IF_REAL   ,"Side junction cap per area"),
 IOP("mjsw",  MOS3_MOD_MJSW,  IF_REAL   ,"Side grading coefficient"),
 IOP("js",    MOS3_MOD_JS,    IF_REAL   ,"Bulk jct. sat. current density"),
 IOP("tox",   MOS3_MOD_TOX,   IF_REAL   ,"Oxide thickness"),
 IOP("ld",    MOS3_MOD_LD,    IF_REAL   ,"Lateral diffusion"),
 IOP("u0",    MOS3_MOD_U0,    IF_REAL   ,"Surface mobility"),
 IOP("uo",    MOS3_MOD_U0,    IF_REAL   ,"Surface mobility"),
 IOP("fc",    MOS3_MOD_FC,    IF_REAL   ,"Forward bias jct. fit parm."),
 IP("nmos",   MOS3_MOD_NMOS,  IF_FLAG   ,"N type MOSfet model"),
 IP("pmos",   MOS3_MOD_PMOS,  IF_FLAG   ,"P type MOSfet model"),
 IOP("nsub",  MOS3_MOD_NSUB,  IF_REAL   ,"Substrate doping"),
 IOP("tpg",   MOS3_MOD_TPG,   IF_INTEGER,"Gate type"),
 IOP("nss",   MOS3_MOD_NSS,   IF_REAL   ,"Surface state density"),
 IOP("delta", MOS3_MOD_DELTA, IF_REAL   ,"Width effect on threshold"),
 IOP("vmax",  MOS3_MOD_VMAX,  IF_REAL   ,"Maximum carrier drift velocity"),
 IOP("xj",    MOS3_MOD_XJ,    IF_REAL   ,"Junction depth"),
 IOP("nfs",   MOS3_MOD_NFS,   IF_REAL   ,"Fast surface state density"),
 IOP("eta",   MOS3_MOD_ETA,   IF_REAL ,""),
 IOP("xd",    MOS3_MOD_XD,    IF_REAL ,"Depletion layer width"),
 IOP("input_delta", MOS3_DELTA, IF_REAL ,"Input delta"),
 IOP("theta", MOS3_MOD_THETA, IF_REAL ,"Theta"),
 IOP("alpha", MOS3_MOD_ALPHA, IF_REAL ,"Alpha"),
 IOP("kappa", MOS3_MOD_KAPPA, IF_REAL ,"Kappa"),
 IOP("tnom",  MOS3_MOD_TNOM,  IF_REAL ,"Parameter measurement temperature"),
};

static char *MOS3names[] = {
    "Drain",
    "Gate",
    "Source",
    "Bulk"
};

SPICEdev MOS3info = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Mos3",
        "Level 3 MOSfet model with Meyer capacitance model",

        sizeof(MOS3names)/sizeof(char *),
        sizeof(MOS3names)/sizeof(char *),
        MOS3names,

        sizeof(MOS3pTable)/sizeof(IFparm),
        MOS3pTable,

        sizeof(MOS3mPTable)/sizeof(IFparm),
        MOS3mPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    MOS3param,
    MOS3mParam,
    MOS3load,
    MOS3setup,
    MOS3setup,
    MOS3temp,
    MOS3trunc,
    NULL,
    MOS3acLoad,
    NULL,
    MOS3destroy,
#ifdef DELETES
    MOS3mDelete,
    MOS3delete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    MOS3getic,
    MOS3ask,
    MOS3mAsk,
    MOS3pzLoad,
#ifdef NEWCONV
    MOS3convTest,
#else /* NEWCONV */
    NULL,
#endif /* NEWCONV */

#ifndef NOSENS
    MOS3sSetup,
    MOS3sLoad,
    MOS3sUpdate,
    MOS3sAcLoad,
    MOS3sPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(MOS3instance),
    sizeof(MOS3model),

};

