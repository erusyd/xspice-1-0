/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include <stdio.h>
#include "INDdefs.h"
#include "suffix.h"

RCSID("IND.c $Revision: 1.4 $ on $Date: 90/11/09 16:34:06 $")

static IFparm INDpTable[] = { /* parameters */ 
 IOP("inductance",IND_IND,    IF_REAL,"Inductance of inductor"),
 IOP("ic",        IND_IC,     IF_REAL,"Initial current through inductor"),
 IP( "sens_ind", IND_IND_SENS,IF_FLAG,
        "flag to request sensitivity WRT inductance"),
 OP( "flux",      IND_FLUX,   IF_REAL,"Flux through inductor"),
 OP( "volt",      IND_VOLT,   IF_REAL,"Terminal voltage of inductor"),
 OP( "current",   IND_CURRENT,IF_REAL,"Current through the inductor"),
 OP( "p",         IND_POWER,  IF_REAL,
        "instantaneous power dissipated by the inductor"),
 OP( "sens_dc", IND_QUEST_SENS_DC,     IF_REAL, "dc sensitivity sensitivity"),
 OP( "sens_real", IND_QUEST_SENS_REAL, IF_REAL, "real part of ac sensitivity"),
 OP( "sens_imag", IND_QUEST_SENS_IMAG, IF_REAL, 
        "dc sensitivity and imag part of ac sensitivty"),
 OP( "sens_mag",  IND_QUEST_SENS_MAG,  IF_REAL, "sensitivity of AC magnitude"),
 OP( "sens_ph",   IND_QUEST_SENS_PH,   IF_REAL, "sensitivity of AC phase"),
 OP( "sens_cplx", IND_QUEST_SENS_CPLX, IF_COMPLEX,    "ac sensitivity")
};

/*static IFparm INDmPTable[] = { /* model parameters */
/*}; /**/

static char *INDnames[] = {
    "L+",
    "L-"
};


#ifdef MUTUAL

static IFparm MUTpTable[] = { /* parameters */ 
 IOP( "coefficient", MUT_COEFF, IF_REAL    , "Mutual inductance"),
 IOP( "inductor1",   MUT_IND1,  IF_INSTANCE, "First coupled inductor"),
 IOP( "inductor2",   MUT_IND2,  IF_INSTANCE, "Second coupled inductor"),
 IP( "sens_coeff",   MUT_COEFF_SENS, IF_FLAG,    
        "flag to request sensitivity WRT coupling factor"),
 OP( "sens_dc",      MUT_QUEST_SENS_DC,   IF_REAL, "dc sensitivity "),
 OP( "sens_real",  MUT_QUEST_SENS_REAL, IF_REAL, "real part of ac sensitivity"),
 OP( "sens_imag",  MUT_QUEST_SENS_IMAG, IF_REAL, 
        "dc sensitivity and imag part of ac sensitivty"),
 OP( "sens_mag",   MUT_QUEST_SENS_MAG,  IF_REAL, "sensitivity of AC magnitude"),
 OP( "sens_ph",    MUT_QUEST_SENS_PH,   IF_REAL, "sensitivity of AC phase"),
 OP( "sens_cplx",  MUT_QUEST_SENS_CPLX, IF_COMPLEX,  "ac sensitivity")
};

/*static IFparm MUTmPTable[] = { /* model parameters */
/*}; /**/

#endif /*MUTUAL*/

SPICEdev INDinfo = {

/* gtri - modify - wbk - 10/11/90 - add 0,NULL entries into IFdevice */
/*                                  structure for MIF element data   */

    {
        "Inductor",
        "Inductors",

        sizeof(INDnames)/sizeof(char *),
        sizeof(INDnames)/sizeof(char *),
        INDnames,

        sizeof(INDpTable)/sizeof(IFparm),
        INDpTable,

        0/*sizeof(INDmPTable)/sizeof(IFparm) /**/,
        NULL/*INDmPTable/**/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },

/* gtri - end - wbk - 10/11/90 */

    INDparam,
    NULL,
    INDload,
    INDsetup,
    INDsetup,
    NULL,
    INDtrunc,
    NULL,
    INDacLoad,
    NULL,
    INDdestroy,
#ifdef DELETES
    INDmDelete,
    INDdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    INDask,
    NULL,
    INDpzLoad,
    NULL,
#ifndef NOSENS
    INDsSetup,
    INDsLoad,
    INDsUpdate,
    INDsAcLoad,
    INDsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(INDinstance),
    sizeof(INDmodel),

};

#ifdef MUTUAL

SPICEdev MUTinfo = {
    {   
        "mutual",
        "Mutual inductors",

        0, /* term count */
        0, /* term count */
        NULL,

        sizeof(MUTpTable)/sizeof(IFparm),
        MUTpTable,

        0/*sizeof(MUTmPTable)/sizeof(IFparm)/**/,
        NULL/*MUTmPTable/**/,

        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
    },


    MUTparam,
    NULL,
    NULL,/* load handled by INDload */
    MUTsetup,
    MUTsetup,
    NULL,
    NULL,
    NULL,
    MUTacLoad,
    NULL,
    MUTdestroy,
#ifdef DELETES
    MUTmDelete,
    MUTdelete,
#else /* DELETES */
    NULL,
    NULL,
#endif /* DELETES */
    NULL,
    MUTask,
    NULL,
    MUTpzLoad,
    NULL,
#ifndef NOSENS
    MUTsSetup,
    NULL,
    NULL,
    NULL,
    MUTsPrint,
    NULL,
#else /* NOSENS */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* NOSENS */

    sizeof(MUTinstance),
    sizeof(MUTmodel),

};

#endif /*MUTUAL*/
