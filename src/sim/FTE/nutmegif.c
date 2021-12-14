
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:12:00 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/nutmegif.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Dummy interface stuff, for nutmeg. This is the easiest way of
 * making sure that nutmeg doesn't try to load spice in also.
 */

#include "prefix.h"
#include "IFsim.h"
#include "SPerror.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEinp.h"
#include "util.h"
#include "suffix.h"

/* for HLP */
char *hlp_filelist[] = {"NUTMEG", ""};

bool if_isnutmeg = true;
bool ft_intrpt = false;
bool ft_setflag = false;

int OUTpBeginPlot() {}
int OUTpData() {}
int OUTwBeginPlot() {}
int OUTwReference() {}
int OUTwData() {}
int OUTwEnd() {}
int OUTendPlot() {}
int OUTbeginDomain() {}
int OUTendDomain() {}
int OUTstopnow() {}
int OUTerror() {}
int OUTattributes() {}

/* ARGSUSED */ int if_run(t, w, s, b) char *t, *w, *b; wordlist *s;
        { return (0); }
/* ARGSUSED */ int if_sens_run(t, w, s, b) char *t, *w, *b; wordlist *s;
        { return (0); }
/* ARGSUSED */ void if_dump(ckt, fp) char *ckt; FILE *fp; { }
/* ARGSUSED */ char * if_inpdeck(deck, tab) struct line *deck; char **tab;
        { return ((char *) 0); }
/* ARGSUSED */ void if_option(ckt, name, type, value) char *name, *ckt, *value;
        int type; { }
/* ARGSUSED */ void if_cktfree(ckt, tab) char *ckt, *tab; { }
/* ARGSUSED */ void if_setndnames(line) char *line; { }
/* ARGSUSED */ char * if_errstring(code) { return ("spice error"); }
/* ARGSUSED */ struct variable * if_getparam(ckt, name, param, ind)
                char *ckt, **name, *param;
                int ind;
        { return ((struct variable *) NULL); }
/* ARGSUSED */ void if_setparam(ckt, name, param, val) 
        char *ckt, *name, *param;  struct variable *val; { }

/* ARGSUSED */
bool
if_tranparams(ckt, start, stop, step) struct circ *ckt;
        double *start, *stop, *step;
{ return (false); }

/* ARGSUSED */ struct variable *if_getstat(n, c) char *n, *c; { return (NULL);}


static IFsimulator SIMinfo = {
    "nutmeg",       /* my name */
    "nutmeg sircuit simulator post processor",  /* more about me */
    "3B.1",         /* my version */

    NULL,       /* newCircuit function */
    NULL,       /* deleteCircuit function */

    NULL,           /* newNode function */ /* NEEDED */
    NULL,           /* groundNode function */
    NULL,           /* bindNode function */
    NULL,           /* findNode function */ /* NEEDED */
    NULL,           /* instToNode function */ /* NEEDED */
    NULL,           /* setNodeParm function */ /* NEEDED */
    NULL,           /* askNodeQuest function */ /* NEEDED */
    NULL,           /* deleteNode function */ /* NEEDED */

    NULL,           /* newInstance function */
    NULL,           /* setInstanceParm function */
    NULL,           /* askInstanceQuest function */
    NULL,           /* findInstance funciton */
    NULL,           /* deleteInstance function */ /* to be added later */

    NULL,           /* newModel function */
    NULL,           /* setModelParm function */
    NULL,           /* askModelQuest function */
    NULL,           /* findModel function */
    NULL,           /* deleteModel function */ /* to be added later */

    NULL,           /* newTask function */
    NULL,           /* newAnalysis function */
    NULL,           /* setAnalysisParm function */
    NULL,           /* askAnalysisQeust function */
    NULL,           /* findAnalysis function */
    NULL,           /* findTask function */
    NULL,           /* deleteTask function */

    NULL,           /* doAnalyses function */

    0,
    NULL,

    0,
    NULL,

    0,
    NULL,

};



/*
 *
 *  And now, our function!
 *
 * (big, isn't it)
 *
 */

/*ARGSUSED*/
int
SIMinit(frontEnd,simulator)
    IFfrontEnd *frontEnd;
    IFsimulator **simulator;
{

    *simulator = &SIMinfo;
    return(OK);
}

    /*
     * INPaName()
     *
     *  Take a parameter by Name and ask for the specified value 
     * *dev is -1 if type unknown, otherwise, device type
     * **fast is a device, and will be set if possible.
     */

/*ARGSUSED*/
int
INPaName(parm,val,ckt,dev,devnam,fast,sim,dataType,selector)
    char *parm;     /* the name of the parameter to set */
    IFvalue *val;   /* the parameter union containing the value to set */
    GENERIC *ckt;   /* the circuit this device is a member of */
    int *dev;       /* the device type code to the device being parsed */
    char *devnam;   /* the name of the device */
    GENERIC **fast;  /* direct pointer to device being parsed */
    IFsimulator *sim;       /* the simulator data structure */
    int *dataType;  /* return the type of the data item. */
    IFvalue *selector;

{
    return(OK);
}

/* insert 'token' into the symbol table */
/*ARGSUSED*/
int
INPinsert(token,tab)
        char **token;
/*        INPtables *tab; */
        char *tab;
{
    return (OK);
}

/*ARGSUSED*/
int
IFnewUid(ckt,newuid,olduid,suffix,type,nodedata) 
    GENERIC *ckt;
    IFuid *newuid;
    IFuid olduid;
    char *suffix;
    int type;
    GENERIC **nodedata;
{
    return(OK);
}

