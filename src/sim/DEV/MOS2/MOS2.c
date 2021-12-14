/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "IFsim.h"
#include "MOS2defs.h"
#include "suffix.h"

RCSID("MOS2.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:22 $")

static IFparm MOS2pTable[] = { /* parameters */ 
 IOP("l",       MOS2_L,     IF_REAL   , "Length"),
 IOP("w",       MOS2_W,     IF_REAL   , "Width"),
 IOP("ad",      MOS2_AD,    IF_REAL   , "Drain area"),
 IOP("as",      MOS2_AS,    IF_REAL   , "Source area"),
 IOP("pd",      MOS2_PD,    IF_REAL   , "Drain perimeter"),
 IOP("ps",      MOS2_PS,    IF_REAL   , "Source perimeter"),
 IOP("nrd",     MOS2_NRD,   IF_REAL   , "Drain squares"),
 IOP("nrs",     MOS2_NRS,   IF_REAL   , "Source squares"),
 IP("off",      MOS2_OFF,   IF_FLAG   , "Device initially off"),
 IOP("icvds",     MOS2_IC_VDS,IF_REAL   , "Initial D-S voltage"),
 IOP("icvgs",     MOS2_IC_VGS,IF_REAL   , "Initial G-S voltage"),
 IOP("icvbs",     MOS2_IC_VBS,IF_REAL   , "Initial B-S voltage"),
 IOP("temp",    MOS2_TEMP,  IF_REAL   , "Instance operating temperature"),
 IP( "ic",      MOS2_IC,    IF_REALVEC, "Vector of D-S, G-S, B-S voltages"),
 IP( "sens_l",  MOS2_L_SENS,IF_FLAG,  "flag to request sensitivity WRT length"),
 IP( "sens_w",  MOS2_W_SENS,IF_FLAG,  "flag to request sensitivity WRT width"),
 OP( "cgs",     MOS2_CGS,   IF_REAL   , "Gate-Source capacitance"),
 OP( "cgd",     MOS2_CGD,   IF_REAL   , "Gate-Drain capacitance"),
 OP( "dnode",   MOS2_DNODE, IF_INTEGER, "Number of drain node"),
 OP( "gnode",   MOS2_GNODE, IF_INTEGER, "Number of gate node"),
 OP( "snode",   MOS2_SNODE, IF_INTEGER, "Number of source node"),
 OP( "bnode",   MOS2_BNODE, IF_INTEGER, "Number of bulk node"),
 OP( "dnodeprime", MOS2_DNODEPRIME, IF_INTEGER, 
        "Number of internal drain node"),
 OP( "snodeprime", MOS2_SNODEPRIME, IF_INTEGER, 
        "Number of internal source node"),
 OP( "sourceconductance", MOS2_SOURCECONDUCT, IF_REAL, "Source conductance"),
 OP( "drainconductance",  MOS2_DRAINCONDUCT,  IF_REAL, "Drain conductance"),
 OP( "von",               MOS2_VON,           IF_REAL,    " "),
 OP( "vdsat",             MOS2_VDSAT,       IF_REAL,"Saturation drain voltage"),
 OP( "sourcevcrit", MOS2_SOURCEVCRIT, IF_REAL,"Critical source voltage"),
 OP( "drainvcrit",  MOS2_DRAINVCRIT,  IF_REAL,"Critical drain voltage"),
 OP( "cd",          MOS2_CD,          IF_REAL,"Drain current"),
 OP( "cbs",     MOS2_CBS,   IF_REAL,    "B-S junction capacitance"),
 OP( "cbd",     MOS2_CBD,   IF_REAL,    "B-D junction capacitance"),
 OP( "gmbs",    MOS2_GMBS,  IF_REAL,    "Bulk-Source transconductance"),
 OP( "gm",      MOS2_GM,    IF_REAL,    "Transconductance"),
 OP( "gds",     MOS2_GDS,   IF_REAL,    "Drain-Source conductance"),
 OP( "gbd",     MOS2_GBD,   IF_REAL,    "Bulk-Drain conductance"),
 OP( "gbs",     MOS2_GBS,   IF_REAL,    "Bulk-Source conductance"),
 OP( "capbd",   MOS2_CAPBD, IF_REAL,    "Bulk-Drain capacitance"),
 OP( "capbs",   MOS2_CAPBS, IF_REAL,    "Bulk-Source capacitance"),
 OP( "cbd0", MOS2_CAPZEROBIASBD, IF_REAL,"Zero-Bias B-D junction capacitance"),
 OP( "cbdsw0",MOS2_CAPZEROBIASBDSW,IF_REAL, " "),
 OP( "cbs0", MOS2_CAPZEROBIASBS, IF_REAL,"Zero-Bias B-S junction capacitance"),
 OP( "cbssw0", MOS2_CAPZEROBIASBSSW,IF_REAL," "),
 OP( "vbd",     MOS2_VBD,   IF_REAL,    "Bulk-Drain voltage"),
 OP( "vbs",     MOS2_VBS,   IF_REAL,    "Bulk-Source voltage"),
 OP( "vgs",     MOS2_VGS,   IF_REAL,    "Gate-Source voltage"),
 OP( "vds",     MOS2_VDS,   IF_REAL,    "Drain-Source voltage"),
 OP( "capgs",   MOS2_CAPGS, IF_REAL,    "Gate-Source capacitance"),
 OP( "qgs",     MOS2_QGS,   IF_REAL,    "Gate-Source charge storage"),
 OP( "cqgs",MOS2_CQGS, IF_REAL,"Capacitance due to gate-source charge storage"),
 OP( "capgd",   MOS2_CAPGD, IF_REAL,    "Gate-Drain capacitance"),
 OP( "qgd",     MOS2_QGD,   IF_REAL,    "Gate-Drain charge storage"),
 OP( "cqgd",MOS2_CQGD,IF_REAL,"Capacitance due to gate-drain charge storage"),
 OP( "capgb",   MOS2_CAPGB, IF_REAL,    "Gate-Bulk capacitance"),
 OP( "qgb",     MOS2_QGB,   IF_REAL,    "Gate-Bulk charge storage"),
 OP( "cqgb",MOS2_CQGB,IF_REAL,"Capacitance due to gate-bulk charge storage"),
 OP( "qbd",     MOS2_QBD,   IF_REAL,    "Bulk-Drain charge storage"),
 OP( "cqbd",MOS2_CQBD,IF_REAL,"Capacitance due to bulk-drain charge storage"),
 OP( "qbs",     MOS2_QBS,   IF_REAL,    "Bulk-Source charge storage"),
 OP( "cqbs",MOS2_CQBS,IF_REAL,"Capacitance due to bulk-source charge storage"),
 OP( "cs",      MOS2_CS,    IF_REAL,    "Source current "),
 OP( "cg",      MOS2_CG,    IF_REAL,    "Gate current "),
 OP( "cb",      MOS2_CB,    IF_REAL,    "Bulk current "),
 OP( "p",       MOS2_POWER, IF_REAL,    "Instantaneous power "),
 OP( "sens_l_dc",         MOS2_L_SENS_DC,       IF_REAL,    
        "dc sensitivity wrt length"),
 OP( "sens_l_real",       MOS2_L_SENS_REAL,     IF_REAL,    
        "real part of ac sensitivity wrt length"),
 OP( "sens_l_imag",       MOS2_L_SENS_IMAG,     IF_REAL,    
        "imag part of ac sensitivity wrt length"),
 OP( "sens_l_cplx",       MOS2_L_SENS_CPLX,     IF_COMPLEX, 
        "ac sensitivity wrt length"),
 OP( "sens_l_mag",        MOS2_L_SENS_MAG,      IF_REAL,    
        "sensitivity wrt l of ac magnitude"),
 OP( "sens_l_ph",         MOS2_L_SENS_PH,       IF_REAL,    
        "sensitivity wrt l of ac phase"),
 OP( "sens_w_dc",         MOS2_W_SENS_DC,       IF_REAL,    
        "dc sensitivity wrt width"),
 OP( "sens_w_real",       MOS2_W_SENS_REAL,     IF_REAL,    
        "dc sensitivity and real part of ac sensitivity wrt width"),
 OP( "sens_w_imag",       MOS2_W_SENS_IMAG,     IF_REAL,    
        "imag part of ac sensitivity wrt width"),
 OP( "sens_w_mag",        MOS2_W_SENS_MAG,      IF_REAL,    
        "sensitivity wrt w of ac magnitude"),
 OP( "sens_w_ph",         MOS2_W_SENS_PH,       IF_REAL,    
        "sensitivity wrt w of ac phase"),
 OP( "sens_w_cplx",       MOS2_W_SENS_CPLX,     IF_COMPLEX,    
        "ac sensitivity wrt width")
};

static IFparm MOS2mPTable[] = { /* model parameters */
 IOP("vto",   MOS2_MOD_VTO,   IF_REAL   ,"Threshold voltage"),
 IOP("vt0",   MOS2_MOD_VTO,   IF_REAL   ,"Threshold voltage"),
 IOP("kp",    MOS2_MOD_KP,    IF_REAL   ,"Transconductance parameter"),
 IOP("gamma", MOS2_MOD_GAMMA, IF_REAL   ,"Bulk threshold parameter"),
 IOP("phi",   MOS2_MOD_PHI,   IF_REAL   ,"Surface potential"),
 IOP("lambda",MOS2_MOD_LAMBDA,IF_REAL   ,"Channel length modulation"),
 IOP("rd",    MOS2_MOD_RD,    IF_REAL   ,"Drain ohmic resistance"),
 IOP("rs",    MOS2_MOD_RS,    IF_REAL   ,"Source ohmic resistance"),
 IOP("cbd",   MOS2_MOD_CBD,   IF_REAL   ,"B-D junction capacitance"),
 IOP("cbs",   MOS2_MOD_CBS,   IF_REAL   ,"B-S junction capacitance"),
 IOP("is",    MOS2_MOD_IS,    IF_REAL   ,"Bulk junction sat. current"),
 IOP("pb",    MOS2_MOD_PB,    IF_REAL   ,"Bulk junction potential"),
 IOP("cgso",  MOS2_MOD_CGSO,  IF_REAL   ,"Gate-source overlap cap."),
 IOP("cgdo",  MOS2_MOD_CGDO,  IF_REAL   ,"Gate-drain overlap cap."),
 IOP("cgbo",  MOS2_MOD_CGBO,  IF_REAL   ,"Gate-bulk overlap cap."),
 IOP("rsh",   MOS2_MOD_RSH,   IF_REAL   ,"Sheet resistance"),
 IOP("cj",    MOS2_MOD_CJ,    IF_REAL   ,"Bottom junction cap per area"),
 IOP("mj",    MOS2_MOD_MJ,    IF_REAL   ,"Bottom grading coefficient"),
 IOP("cjsw",  MOS2_MOD_CJSW,  IF_REAL   ,"Side junction cap per area"),
 IOP("mjsw",  MOS2_MOD_MJSW,  IF_REAL   ,"Side grading coefficient"),
 IOP("js",    MOS2_MOD_JS,    IF_REAL   ,"Bulk jct. sat. current density"),
 IOP("tox",   MOS2_MOD_TOX,   IF_REAL   ,"Oxide thickness"),
 IOP("ld",    MOS2_MOD_LD,    IF_REAL   ,"Lateral diffusion"),
 IOP("u0",    MOS2_MOD_U0,    IF_REAL   ,"Surface mobility"),
 IOP("uo",    MOS2_MOD_U0,    IF_REAL   ,"Surface mobility"),
 IOP("fc",    MOS2_MOD_FC,    IF_REAL   ,"Forward bias jct. fit parm."),
 IP("nmos",   MOS2_MOD_NMOS,  IF_FLAG   ,"N type MOSfet model"),
 IP("pmos",   MOS2_MOD_PMOS,  IF_FLAG   ,"P type MOSfet model"),
 IOP("nsub",  MOS2_MOD_NSUB,  IF_REAL   ,"Substrate doping"),
 IOP("tpg",   MOS2_MOD_TPG,   IF_INTEGER,"Gate type"),
 IOP("nss",   MOS2_MOD_NSS,   IF_REAL   ,"Surface state density"),
 IOP("delta", MOS2_MOD_DELTA, IF_REAL   ,"Width effect on threshold"),
 IOP("uexp",  MOS2_MOD_UEXP,  IF_REAL   ,"Crit. field exp for mob. deg."),
 IOP("ucrit", MOS2_MOD_UCRIT, IF_REAL   ,"Crit. field for mob. degradation"),
 IOP("vmax",  MOS2_MOD_VMAX,  IF_REAL   ,"Maximum carrier drift velocity"),
 IOP("xj",    MOS2_MOD_XJ,    IF_REAL   ,"Junction depth"),
 IOP("neff",  MOS2_MOD_NEFF,  IF_REAL   ,"Total channel charge coeff."),
 IOP("nfs",   MOS2_MOD_NFS,   IF_REAL   ,"Fast surface state density"),
 IOP("tnom",  MOS2_MOD_TNOM,  IF_REAL   ,"parameter measurement temperature")
};

static char *MOS2names[] = {
    "Drain",
    "Gate",
    "Source",
    "Bulk"
};

SPICEdev MOS2info = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Mos2",
        "Level 2 MOSfet model with Meyer capacitance model",

        sizeof(MOS2names)/sizeof(char *),
        sizeof(MOS2names)/sizeof(char *),
        MOS2names,

        sizeof(MOS2pTable)/sizeof(IFparm),
        MOS2pTable,

        sizeof(MOS2mPTable)/sizeof(IFparm),
        MOS2mPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    MOS2param,
    MOS2mParam,
    MOS2load,
    MOS2setup,
    MOS2setup,
    MOS2temp,
    MOS2trunc,
    NULL,
    MOS2acLoad,
    NULL,
    MOS2destroy,
#ifdef DELETES
    MOS2mDelete,
    MOS2delete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    MOS2getic,
    MOS2ask,
    NULL,
    MOS2pzLoad,
#ifdef NEWCONV
    MOS2convTest,
#else /* NEWCONV */
    NULL,
#endif /* NEWCONV */

#ifndef NOSENS
    MOS2sSetup,
    MOS2sLoad,
    MOS2sUpdate,
    MOS2sAcLoad,
    MOS2sPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(MOS2instance),
    sizeof(MOS2model),
};

