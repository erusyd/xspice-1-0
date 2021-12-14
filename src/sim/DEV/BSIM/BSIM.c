/*
 * Copyright (c) 1985 Hong J. Park, Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "DEVdefs.h"
#include "BSIMdefs.h"
#include "suffix.h"

RCSID("BSIM.c $Revision: 1.4 $ on $Date: 90/11/09 16:33:49 $")

static IFparm BSIMpTable[] = { /* parameters */
 IOP( "l",   BSIM_L,      IF_REAL   , "Length"),
 IOP( "w",   BSIM_W,      IF_REAL   , "Width"),
 IOP( "ad",  BSIM_AD,     IF_REAL   , "Drain area"),
 IOP( "as",  BSIM_AS,     IF_REAL   , "Source area"),
 IOP( "pd",  BSIM_PD,     IF_REAL   , "Drain perimeter"),
 IOP( "ps",  BSIM_PS,     IF_REAL   , "Source perimeter"),
 IOP( "nrd", BSIM_NRD,    IF_REAL   , "Number of squares in drain"),
 IOP( "nrs", BSIM_NRS,    IF_REAL   , "Number of squares in source"),
 IOP( "off", BSIM_OFF,    IF_FLAG   , "Device is initially off"),
 IOP( "vds", BSIM_IC_VDS, IF_REAL   , "Initial D-S voltage"),
 IOP( "vgs", BSIM_IC_VGS, IF_REAL   , "Initial G-S voltage"),
 IOP( "vbs", BSIM_IC_VBS, IF_REAL   , "Initial B-S voltage"),
 IP( "ic",  BSIM_IC,     IF_VECTOR , "Vector of DS,GS,BS initial voltages")
};

static IFparm BSIMmPTable[] = { /* model parameters */
 IOP( "vfb",   BSIM_MOD_VFB0,      IF_REAL,"Flat band voltage"),
 IOP( "lvfb",  BSIM_MOD_VFBL,      IF_REAL,
                    "Length dependence of vfb"),
 IOP( "wvfb",  BSIM_MOD_VFBW,      IF_REAL,
                    "Width dependence of vfb"),
 IOP( "phi",   BSIM_MOD_PHI0,      IF_REAL,
        "Strong inversion surface potential "),
 IOP( "lphi",  BSIM_MOD_PHIL,      IF_REAL,
        "Length dependence of phi"),
 IOP( "wphi",  BSIM_MOD_PHIW,      IF_REAL,
        "Width dependence of phi"),
 IOP( "k1",    BSIM_MOD_K10,       IF_REAL,
        "Bulk effect coefficient 1"),
 IOP( "lk1",   BSIM_MOD_K1L,       IF_REAL,
        "Length dependence of k1"),
 IOP( "wk1",   BSIM_MOD_K1W,       IF_REAL,
        "Width dependence of k1"),
 IOP( "k2",    BSIM_MOD_K20,       IF_REAL,
        "Bulk effect coefficient 2"),
 IOP( "lk2",   BSIM_MOD_K2L,       IF_REAL,
        "Length dependence of k2"),
 IOP( "wk2",   BSIM_MOD_K2W,       IF_REAL,
        "Width dependence of k2"),
 IOP( "eta",   BSIM_MOD_ETA0,      IF_REAL,
        "VDS dependence of threshold voltage"),
 IOP( "leta",  BSIM_MOD_ETAL,      IF_REAL,
        "Length dependence of eta"),
 IOP( "weta",  BSIM_MOD_ETAW,      IF_REAL,
        "Width dependence of eta"),
 IOP( "x2e",   BSIM_MOD_ETAB0,     IF_REAL,
        "VBS dependence of eta"),
 IOP( "lx2e",  BSIM_MOD_ETABL,     IF_REAL,
        "Length dependence of x2e"),
 IOP( "wx2e",  BSIM_MOD_ETABW,     IF_REAL,
        "Width dependence of x2e"),
 IOP( "x3e",   BSIM_MOD_ETAD0,     IF_REAL,
        "VDS dependence of eta"),
 IOP( "lx3e",  BSIM_MOD_ETADL,     IF_REAL,
        "Length dependence of x3e"),
 IOP( "wx3e",  BSIM_MOD_ETADW,     IF_REAL,
        "Width dependence of x3e"),
 IOP( "dl",    BSIM_MOD_DELTAL,    IF_REAL,
        "Channel length reduction in um"),
 IOP( "dw",    BSIM_MOD_DELTAW,    IF_REAL,
        "Channel width reduction in um"),
 IOP( "muz",   BSIM_MOD_MOBZERO,   IF_REAL,
        "Zero field mobility at VDS=0 VGS=VTH"),
 IOP( "x2mz",  BSIM_MOD_MOBZEROB0, IF_REAL,
        "VBS dependence of muz"),
 IOP( "lx2mz", BSIM_MOD_MOBZEROBL, IF_REAL,
        "Length dependence of x2mz"),
 IOP( "wx2mz", BSIM_MOD_MOBZEROBW, IF_REAL,
        "Width dependence of x2mz"),
 IOP( "mus",   BSIM_MOD_MOBVDD0,   IF_REAL,
        "Mobility at VDS=VDD VGS=VTH, channel length modulation"),
 IOP( "lmus",  BSIM_MOD_MOBVDDL,   IF_REAL,
        "Length dependence of mus"),
 IOP( "wmus",  BSIM_MOD_MOBVDDW,   IF_REAL,
        "Width dependence of mus"),
 IOP( "x2ms",  BSIM_MOD_MOBVDDB0,  IF_REAL,
            "VBS dependence of mus"),
 IOP( "lx2ms", BSIM_MOD_MOBVDDBL,  IF_REAL,
        "Length dependence of x2ms"),
 IOP( "wx2ms", BSIM_MOD_MOBVDDBW,  IF_REAL,
        "Width dependence of x2ms"),
 IOP( "x3ms",  BSIM_MOD_MOBVDDD0,  IF_REAL,
        "VDS dependence of mus"),
 IOP( "lx3ms", BSIM_MOD_MOBVDDDL,  IF_REAL,
        "Length dependence of x3ms"),
 IOP( "wx3ms", BSIM_MOD_MOBVDDDW,  IF_REAL,
        "Width dependence of x3ms"),
 IOP( "u0",    BSIM_MOD_UGS0,      IF_REAL,
        "VGS dependence of mobility"),
 IOP( "lu0",   BSIM_MOD_UGSL,      IF_REAL,
                "Length dependence of u0"),
 IOP( "wu0",   BSIM_MOD_UGSW,      IF_REAL,
        "Width dependence of u0"),
 IOP( "x2u0",  BSIM_MOD_UGSB0,     IF_REAL,
        "VBS dependence of u0"),
 IOP( "lx2u0", BSIM_MOD_UGSBL,     IF_REAL,
        "Length dependence of x2u0"),
 IOP( "wx2u0", BSIM_MOD_UGSBW,     IF_REAL,
        "Width dependence of x2u0"),
 IOP( "u1",    BSIM_MOD_UDS0,      IF_REAL,
        "VDS depence of mobility, velocity saturation"),
 IOP( "lu1",   BSIM_MOD_UDSL,      IF_REAL,
        "Length dependence of u1"),
 IOP( "wu1",   BSIM_MOD_UDSW,      IF_REAL,
        "Width dependence of u1"),
 IOP( "x2u1",  BSIM_MOD_UDSB0,     IF_REAL,
        "VBS depence of u1"),
 IOP( "lx2u1", BSIM_MOD_UDSBL,     IF_REAL,
        "Length depence of x2u1"),
 IOP( "wx2u1", BSIM_MOD_UDSBW,     IF_REAL,
        "Width depence of x2u1"),
 IOP( "x3u1",  BSIM_MOD_UDSD0,     IF_REAL,
        "VDS depence of u1"),
 IOP( "lx3u1", BSIM_MOD_UDSDL,     IF_REAL,
        "Length dependence of x3u1"),
 IOP( "wx3u1", BSIM_MOD_UDSDW,     IF_REAL,
        "Width depence of x3u1"),
 IOP( "n0",    BSIM_MOD_N00,       IF_REAL,
            "Subthreshold slope"),
 IOP( "ln0",   BSIM_MOD_N0L,       IF_REAL,
        "Length dependence of n0"),
 IOP( "wn0",   BSIM_MOD_N0W,       IF_REAL,
        "Width dependence of n0"),
 IOP( "nb",    BSIM_MOD_NB0,       IF_REAL,
        "VBS dependence of subthreshold slope"),
 IOP( "lnb",   BSIM_MOD_NBL,       IF_REAL,
        "Length dependence of nb"),
 IOP( "wnb",   BSIM_MOD_NBW,       IF_REAL,
        "Width dependence of nb"),
 IOP( "nd",    BSIM_MOD_ND0,       IF_REAL,
        "VDS dependence of subthreshold slope"),
 IOP( "lnd",   BSIM_MOD_NDL,       IF_REAL,
        "Length dependence of nd"),
 IOP( "wnd",   BSIM_MOD_NDW,       IF_REAL,
        "Width dependence of nd"),
 IOP( "tox",   BSIM_MOD_TOX,       IF_REAL,
        "Gate oxide thickness in um"),
 IOP( "temp",  BSIM_MOD_TEMP,      IF_REAL,
        "Temperature in degree Celcius"),
 IOP( "vdd",   BSIM_MOD_VDD,       IF_REAL,
                "Supply voltage to specify mus"),
 IOP( "cgso",  BSIM_MOD_CGSO,      IF_REAL,
          "Gate source overlap capacitance per unit channel width(m)"),
 IOP( "cgdo",  BSIM_MOD_CGDO,      IF_REAL,
          "Gate drain overlap capacitance per unit channel width(m)"),
 IOP( "cgbo",  BSIM_MOD_CGBO,      IF_REAL,
          "Gate bulk overlap capacitance per unit channel length(m)"),
 IOP( "xpart", BSIM_MOD_XPART,     IF_REAL,
      "Flag for channel charge partitioning"),
 IOP( "rsh",   BSIM_MOD_RSH,       IF_REAL,
      "Source drain diffusion sheet resistance in ohm per square"),
 IOP( "js",    BSIM_MOD_JS,        IF_REAL,
      "Source drain junction saturation current per unit area"),
 IOP( "pb",    BSIM_MOD_PB,        IF_REAL,
      "Source drain junction built in potential"),
 IOP( "mj",    BSIM_MOD_MJ,        IF_REAL,
       "Source drain bottom junction capacitance grading coefficient"),
 IOP( "pbsw",  BSIM_MOD_PBSW,      IF_REAL,
       "Source drain side junction capacitance built in potential"),
 IOP( "mjsw",  BSIM_MOD_MJSW,      IF_REAL,
       "Source drain side junction capacitance grading coefficient"),
 IOP( "cj",    BSIM_MOD_CJ,        IF_REAL,
       "Source drain bottom junction capacitance per unit area"),
 IOP( "cjsw",  BSIM_MOD_CJSW,      IF_REAL,
       "Source drain side junction capacitance per unit area"),
 IOP( "wdf",   BSIM_MOD_DEFWIDTH,  IF_REAL,
       "Default width of source drain diffusion in um"),
 IOP( "dell",  BSIM_MOD_DELLENGTH, IF_REAL,
       "Length reduction of source drain diffusion"),
 IP( "nmos",  BSIM_MOD_NMOS,      IF_FLAG,
       "Flag to indicate NMOS"),
 IP( "pmos",  BSIM_MOD_PMOS,      IF_FLAG,
       "Flag to indicate PMOS"),
};

static char *BSIMnames[] = {
    "Drain",
    "Gate",
    "Source",
    "Bulk"
};

SPICEdev BSIMinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {   "BSIM",
        "Berkeley Short Channel IGFET Model",

        sizeof(BSIMnames)/sizeof(char *),
        sizeof(BSIMnames)/sizeof(char *),
        BSIMnames,

        sizeof(BSIMpTable)/sizeof(IFparm),
        BSIMpTable,

        sizeof(BSIMmPTable)/sizeof(IFparm),
        BSIMmPTable,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    BSIMparam,
    BSIMmParam,
    BSIMload,
    BSIMsetup,
    BSIMsetup,
    BSIMtemp,
    BSIMtrunc,
    NULL,
    BSIMacLoad,
    NULL,
    BSIMdestroy,
#ifdef DELETES
    BSIMmDelete,
    BSIMdelete, 
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    BSIMgetic,
    BSIMask,
    BSIMmAsk,
    BSIMpzLoad,
#ifdef NEWCONV
    BSIMconvTest,
#else /* NEWCONV */
    NULL,
#endif /* NEWCONV */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    sizeof(BSIMinstance),
    sizeof(BSIMmodel),

};
