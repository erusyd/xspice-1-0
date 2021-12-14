/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "DEVdefs.h"
#include "JOBdefs.h"
#include "util.h"
#include "SPerror.h"
#include "OPTdefs.h"
#include "CONST.h" /* we initialize & allocate memory for these here */

/* gtri - evt - wbk - 5/20/91 - Add stuff for user-defined nodes */
#include "EVTudn.h"
/* gtri - end - wbk - 5/20/91 - Add stuff for user-defined nodes */


#include "suffix.h"

RCSID("SPIinit.c $Revision: 1.8 $ on $Date: 92/09/18 10:47:23 $")

extern SPICEdev ASRCinfo;
extern SPICEdev VSRCinfo;
extern SPICEdev ISRCinfo;
extern SPICEdev URCinfo;
extern SPICEdev VCVSinfo;
extern SPICEdev CCVSinfo;
extern SPICEdev VCCSinfo;
extern SPICEdev CCCSinfo;
extern SPICEdev RESinfo;
extern SPICEdev TRAinfo;
extern SPICEdev CAPinfo;
extern SPICEdev INDinfo;
extern SPICEdev MUTinfo;
extern SPICEdev MOS1info;
extern SPICEdev MOS2info;
extern SPICEdev MOS3info;
extern SPICEdev DIOinfo;
extern SPICEdev BJTinfo;
extern SPICEdev JFETinfo;
extern SPICEdev BSIMinfo;
extern SPICEdev CSWinfo;
extern SPICEdev SWinfo;
extern SPICEdev MESinfo;
extern SPICEdev icm_poly_info;

/* gtri - add - wbk - 11/10/90 - include file with externs for */
/*                               models to be added            */
#include "CMextrn.h"
/* gtri - end - wbk - 11/10/90 */

extern SPICEanalysis ACinfo;
extern SPICEanalysis OPTinfo;
extern SPICEanalysis DCTinfo;
extern SPICEanalysis DCOinfo;
extern SPICEanalysis TRANinfo;
extern SPICEanalysis PZinfo;
extern SPICEanalysis SENinfo;
extern SPICEanalysis TFinfo;

static char * specSigList[] = {
    "time"
};

static IFparm nodeParms[] = {
    IP( "nodeset",PARM_NS ,IF_REAL,"suggested initial voltage"),
    IP( "ic",PARM_IC ,IF_REAL,"initial voltage"),
    IP( "type",PARM_NODETYPE ,IF_INTEGER,"output type of equation")
};


SPICEanalysis *analInfo[] = {
    &OPTinfo,
    &ACinfo,
    &DCTinfo,
    &DCOinfo,
    &TRANinfo,
    &PZinfo,
    &SENinfo,
    &TFinfo,
};

SPICEdev *DEVices[] = {

        &ASRCinfo,
        &VSRCinfo,
        &ISRCinfo,
        &URCinfo,
        &VCVSinfo,
        &CCVSinfo,
        &VCCSinfo,
        &CCCSinfo,
        &RESinfo,
        &TRAinfo,
        &CAPinfo,
        &INDinfo,
        &MUTinfo,
        &MOS1info,
        &MOS2info,
        &MOS3info,
        &DIOinfo,
        &BJTinfo,
        &JFETinfo,
        &BSIMinfo,
        &CSWinfo,
        &MESinfo,
        &SWinfo,
        &icm_poly_info,

/* gtri - add - wbk - 11/10/90 - include file with pointers for */
/*                               models to be added            */
#include "CMinfo.h"
/* gtri - end - wbk - 11/10/90 */

};


static IFsimulator SIMinfo = {

/* gtri - begin - change name of program to XSPICE */
    "XSPICE",       /* my name */
    "XSPICE circuit level simulation program",  /* more about me */
    "1.0",         /* my version */
/* gtri - end - change name of program to XSPICE */

    CKTinit,        /* newCircuit function */
    CKTdestroy,     /* deleteCircuit function */

    CKTnewNode,     /* newNode function */
    CKTground,      /* groundNode function */
    CKTbindNode,    /* bindNode function */
    CKTfndNode,     /* findNode function */
    CKTinst2Node,   /* instToNode function */
    CKTsetNodPm,    /* setNodeParm function */
    CKTaskNodQst,   /* askNodeQuest function */
    CKTdltNod,      /* deleteNode function */

    CKTcrtElt,      /* newInstance function */
    CKTparam,       /* setInstanceParm function */
    CKTask,         /* askInstanceQuest function */
    CKTfndDev,      /* findInstance funciton */
    CKTdltInst,     /* deleteInstance function */

    CKTmodCrt,      /* newModel function */
    CKTmodParam,    /* setModelParm function */
    CKTmodAsk,      /* askModelQuest function */
    CKTfndMod,      /* findModel function */
    CKTdltMod,      /* deleteModel function */

    CKTnewTask,     /* newTask function */
    CKTnewAnal,     /* newAnalysis function */
    CKTsetAnalPm,   /* setAnalysisParm function */
    CKTaskAnalQ,    /* askAnalysisQeust function */
    CKTfndAnal,     /* findAnalysis function */
    CKTfndTask,     /* findTask function */
    CKTdelTask,     /* deleteTask function */

    CKTdoJob,       /* doAnalyses function */

    sizeof(DEVices)/sizeof(SPICEdev *),
    (IFdevice**)DEVices,

    sizeof(analInfo)/sizeof(SPICEanalysis *),
    (IFanalysis **)analInfo,

    sizeof(nodeParms)/sizeof(IFparm),
    nodeParms,

    sizeof(specSigList)/sizeof(char *),
    specSigList,

};



/* my internal global constant for number of devices */
int DEVmaxnum = sizeof(DEVices)/sizeof(SPICEdev *);
int ANALmaxnum = sizeof(analInfo)/sizeof(SPICEanalysis*);

/* allocate space for global constants in 'CONST.h' */

double CONSTroot2;
double CONSTvt0;
double CONSTKoverQ;
double CONSTe;
IFfrontEnd *SPfrontEnd = NULL;

/* gtri - evt - wbk - 5/20/91 - Add stuff for user-defined nodes */

extern Evt_Udn_Info_t idn_digital_info;
#include "UDNextrn.h"

Evt_Udn_Info_t  *g_evt_udn_info[] = {
    &idn_digital_info,
#include "UDNinfo.h"
};

int g_evt_num_udn_types = sizeof(g_evt_udn_info)/sizeof(Evt_Udn_Info_t *);

/* gtri - end - wbk - 5/20/91 - Add stuff for user-defined nodes */


/*
 *
 *  And now, our function!
 *
 * (big, isn't it)
 *
 */

int
SPIinit(frtEnd,description)
    IFfrontEnd *frtEnd;
    IFsimulator **description;
{

    SPfrontEnd = frtEnd;
    *description = &SIMinfo;
    CONSTroot2 = sqrt(2.);
    CONSTvt0 = CONSTboltz * (27 /* deg c */ + CONSTCtoK ) / CHARGE;
    CONSTKoverQ = CONSTboltz / CHARGE;
    CONSTe = exp((double)1.0);
    return(OK);
}
