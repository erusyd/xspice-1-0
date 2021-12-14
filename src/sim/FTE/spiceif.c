
/* RCS Info: $Revision: 1.8 $ on $Date: 92/09/18 14:08:14 $
 *           $Source: //carob/xspice/src/sim/FTE/RCS/spiceif.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Interface routines. These are specific to spice. The only changes to FTE
 * that should be needed to make FTE work with a different simulator is
 * to rewrite this file. What each routine is expected to do can be
 * found in the programmer's manual. This file should be the only one
 * that includes spice header files.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEinp.h"
#ifndef CMS
#include "FTEconstant.h"
#else  /* CMS */
#include "FTEconst.h"
#endif /* CMS */
#include "INPdefs.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "IFsim.h"
#include "util.h"

/* gtri - add - wbk - 11/9/90 - include MIF function prototypes */
#include "MIFproto.h"
/* gtri - end - wbk - 11/9/90 */

/* gtri - evt - wbk - 5/20/91 - Add stuff for user-defined nodes */
#include "EVTproto.h"
#include "EVTudn.h"
/* gtri - end - wbk - 5/20/91 - Add stuff for user-defined nodes */

#include "suffix.h"

/* for HLP */
char *hlp_filelist[] = {"NUTMEG", "SPICE", "" };

bool if_isnutmeg = false;

static IFvalue *doask();
static int finddev();
static IFparm * parmlookup();
static struct variable *parmtovar();

/* Stuff from INP and DEV... */


/* Input a single deck, and return a pointer to the circuit. */

char *
if_inpdeck(deck, tab)
    struct line *deck;
    char **tab;
{
    GENERIC *ckt;
    int err, i, j;
    struct line *ll;
    IFuid taskUid;
    IFuid optUid;
    int which = -1;

    for (i = 0, ll = deck; ll; ll = ll->li_next)
        i++;
    *tab = (char *) INPtabInit(i);
    ft_curckt->ci_symtab = *tab;
    if ((err = (*(ft_sim->newCircuit))(&ckt))
            != OK) {
        ft_sperror(err, "CKTinit");
        return (NULL);
    }
    err = IFnewUid(ckt,&taskUid,(IFuid)NULL,"default",UID_TASK,(GENERIC**)NULL);
    if(err) {
        ft_sperror(err,"newUid");
        return(NULL);
    }
    err = (*(ft_sim->newTask))(ckt,(GENERIC**)&(ft_curckt->ci_defTask),taskUid);
    if(err) {
        ft_sperror(err,"newTask");
        return(NULL);
    }
    for(j=0;j<ft_sim->numAnalyses;j++) {
        if(strcmp(ft_sim->analyses[j]->name,"options")==0) {
            which = j;
            break;
        }
    } 
    if(which != -1) {
        err = IFnewUid(ckt,&optUid,(IFuid)NULL,"options",UID_ANALYSIS,
                (GENERIC**)NULL);
        if(err) {
            ft_sperror(err,"newUid");
            return(NULL);
        }
        err = (*(ft_sim->newAnalysis))(ft_curckt->ci_ckt,which,optUid,
                (GENERIC**)&(ft_curckt->ci_defOpt),
                (GENERIC*)ft_curckt->ci_defTask);
        if(err) {
            ft_sperror(err,"createOptions");
            return(NULL);
        }
        ft_curckt->ci_curOpt  = ft_curckt->ci_defOpt;
    }
    ft_curckt->ci_curTask = ft_curckt->ci_defTask;

    INPpas1((GENERIC *) ckt, (card *) deck->li_next,(INPtables *)*tab);
    INPpas2((GENERIC *) ckt, (card *) deck->li_next,
            (INPtables *) *tab,ft_curckt->ci_defTask);
    INPkillMods();

/* gtri - begin - wbk - 6/6/91 - Finish initialization of event driven structures */
    err = EVTinit((void *) ckt);
    if(err) {
        ft_sperror(err,"EVTinit");
        return(NULL);
    }
/* gtri - end - wbk - 6/6/91 - Finish initialization of event driven structures */

    return (ckt);
}

/* Do a run of the circuit, of the given type. Type "resume" is special --
 * it means to resume whatever simulation that was in progress. The
 * return value of this routine is 0 if the exit was ok, and 1 if there was
 * a reason to interrupt the circuit (interrupt typed at the keyboard,
 * error in the simulation, etc). args should be the entire command line,
 * e.g. "tran 1 10 20 uic"
 */

int
if_run(t, what, args, tab)
    char *t;
    char *what;
    wordlist *args;
    char *tab;
{
    GENERIC *ckt = (GENERIC *) t;
    int err;
    struct line deck;
    char buf[BSIZE];
    int j;
    int which = -1;
    IFuid specUid,optUid;

    /* First parse the line... */
    if (eq(what, "tran") || eq(what, "ac") || eq(what, "dc") || 
            eq(what, "op") || eq(what, "pz")) {
        (void) sprintf(buf, ".%s", wl_flatten(args));
        deck.li_next = deck.li_actual = NULL;
        deck.li_error = NULL;
        deck.li_linenum = 0;
        deck.li_line = buf;
        if(ft_curckt->ci_specTask) {
            err=(*(ft_sim->deleteTask))(ft_curckt->ci_ckt,
                    ft_curckt->ci_specTask);
            if(err) {
                ft_sperror(err,"deleteTask");
                return(1);
            }
        }
        err = IFnewUid(ft_curckt->ci_ckt,&specUid,(IFuid)NULL,"special",
                UID_TASK,(GENERIC**)NULL);
        if(err) {
            ft_sperror(err,"newUid");
            return(1);
        }
        err = (*(ft_sim->newTask))(ft_curckt->ci_ckt,
                (GENERIC**)&(ft_curckt->ci_specTask),specUid);
        if(err) {
            ft_sperror(err,"newTask");
            return(1);
        }
        for(j=0;j<ft_sim->numAnalyses;j++) {
            if(strcmp(ft_sim->analyses[j]->name,"options")==0) {
                which = j;
                break;
            }
        } 
        if(which != -1) {
            err = IFnewUid(ft_curckt->ci_ckt,&optUid,(IFuid)NULL,"options",
                    UID_ANALYSIS,(GENERIC**)NULL);
            if(err) {
                ft_sperror(err,"newUid");
                return(1);
            }
            err = (*(ft_sim->newAnalysis))(ft_curckt->ci_ckt,which,optUid,
                    (GENERIC**)&(ft_curckt->ci_specOpt),
                    (GENERIC*)ft_curckt->ci_specTask);
            if(err) {
                ft_sperror(err,"createOptions");
                return(0);
            }
            ft_curckt->ci_curOpt  = ft_curckt->ci_specOpt;
        }
        ft_curckt->ci_curTask = ft_curckt->ci_specTask;
        INPpas2(ckt, (card *) &deck, (INPtables *)tab, ft_curckt->ci_specTask);
        if (deck.li_error) {
            /* INP produdes an E_EXISTS error here... Don't
             * make this fatal...
             */
            fprintf(cp_err, "Warning: %s\n", deck.li_error);
        }
    }
    if( eq(what,"run") ) {
        ft_curckt->ci_curTask = ft_curckt->ci_defTask;
        ft_curckt->ci_curOpt = ft_curckt->ci_defOpt;
    }

    /* Then do the stuff. */
    if (  (eq(what, "tran"))  ||
          (eq(what, "ac"))  ||
          (eq(what, "dc")) ||
          (eq(what, "op")) ||
          (eq(what, "pz")) ||
          (eq(what, "run"))     )  {
        if ((err = (*(ft_sim->doAnalyses))(ckt, 1, ft_curckt->ci_curTask))!=OK){
            ft_sperror(err, "doAnalyses");
            /* wrd_end(); */
            return (1);
        }
    } else if (eq(what, "resume")) {
        if ((err = (*(ft_sim->doAnalyses))(ckt, 0, ft_curckt->ci_curTask))!=OK){
            ft_sperror(err, "doAnalyses");
            /* wrd_end(); */
            return (1);
        }
    } else {
        fprintf(cp_err, "if_run: Internal Error: bad run type %s\n",
                what);
    }
    return (0);
}

/* Set an option in the circuit. Arguments are option name, type, and
 * value (the last a char *), suitable for casting to whatever needed...
 */

static char *unsupported[] = {
    "itl3",
    "itl5",
    "lvltim",
    "maxord",
    "method",
    NULL
} ;

static char *obsolete[] = {
    "limpts",
    "limtim",
    "lvlcod",
    "nomod",
    NULL
} ;

void
if_option(ckt, name, type, value)
    char *ckt;
    char *name;
    int type;
    char *value;
{
    IFvalue pval;
    int err, i;
    GENERIC *cc = (GENERIC *) ckt;
    char **vv;
    int which = -1;

    if (eq(name, "acct")) {
        ft_acctprint = true;
	return;
    } else if (eq(name, "list")) {
        ft_listprint = true;
	return;
    } else if (eq(name, "node")) {
        ft_nodesprint = true;
	return;
    } else if (eq(name, "opts")) {
        ft_optsprint = true;
	return;
    } else if (eq(name, "nopage")) {
	ft_nopage = true;
	return;
    }

    for(i=0;i<ft_sim->numAnalyses;i++) {
        if(strcmp(ft_sim->analyses[i]->name,"options")==0) {
            which = i;
            break;
        }
    }
    if(which==-1) {
        fprintf(cp_err,"Warning:  .options card unsupported\n");
        return;
    }

    for (i = 0; i < ft_sim->analyses[which]->numParms; i++)
        if (eq(ft_sim->analyses[which]->analysisParms[i].keyword, name) &&
                (ft_sim->analyses[which]->analysisParms[i].dataType & IF_SET))
            break;
    if (i == ft_sim->analyses[which]->numParms) {
        /* See if this is unsupported or obsolete. */
        for (vv = unsupported; *vv; vv++)
            if (eq(name, *vv)) {
                fprintf(cp_err, 
            "Warning: option %s is currently unsupported.\n", name);
                return;
            }
        for (vv = obsolete; *vv; vv++)
            if (eq(name, *vv)) {
                fprintf(cp_err, 
                "Warning: option %s is obsolete.\n", name);
                return;
            }
        return;
    }

#ifndef LINT
    switch (ft_sim->analyses[which]->analysisParms[i].dataType & IF_VARTYPES) {
#else
    switch (i) {
#endif
        case IF_REAL:
            if (type == VT_REAL)
                pval.rValue = *((double *) value);
            else if (type == VT_NUM)
                pval.rValue = *((int *) value);
            else
                goto badtype;
            break;
        case IF_INTEGER:
            if (type == VT_NUM)
                pval.iValue = *((int *) value);
            else if (type == VT_REAL)
                pval.iValue = *((double *) value);
            else
                goto badtype;
            break;
        case IF_STRING:
            if (type == VT_STRING)
                pval.sValue = copy(value);
            else
                goto badtype;
            break;
        case IF_FLAG:
            /* Do nothing. */
            pval.iValue = 1;
            break;
        default:
            fprintf(cp_err, 
            "if_option: Internal Error: bad option type %d.\n",
                    ft_sim->analyses[which]->analysisParms[i].dataType);
    }

    if ((err = (*(ft_sim->setAnalysisParm))(cc, (GENERIC *)ft_curckt->ci_curOpt,
            ft_sim->analyses[which]->analysisParms[i].id, &pval,
            (IFvalue *)NULL)) != OK)
        ft_sperror(err, "setAnalysisParm(options)");
    return;
badtype:
    fprintf(cp_err, "Error: bad type given for option %s --\n", name);
    fprintf(cp_err, "\ttype given was ");
    switch (type) {
        case VT_BOOL:   fputs("boolean", cp_err); break;
        case VT_NUM:    fputs("integer", cp_err); break;
        case VT_REAL:   fputs("real", cp_err); break;
        case VT_STRING: fputs("string", cp_err); break;
        case VT_LIST:   fputs("list", cp_err); break;
        default:    fputs("something strange", cp_err); break;
    }
    fprintf(cp_err, ", type expected was ");
#ifndef LINT
    switch(ft_sim->analyses[which]->analysisParms[i].dataType & IF_VARTYPES) {
#else
    switch (i) {
#endif
        case IF_REAL:   fputs("real.\n", cp_err); break;
        case IF_INTEGER:fputs("integer.\n", cp_err); break;
        case IF_STRING: fputs("string.\n", cp_err); break;
        case IF_FLAG:   fputs("flag.\n", cp_err); break;
        default:    fputs("something strange.\n", cp_err); break;
    }
    if (type == VT_BOOL)
fputs("\t(Note that you must use an = to separate option name and value.)\n", 
                    cp_err); 
    return;
}

/* ARGSUSED */
void
if_dump(ckt, file)
    char *ckt;
    FILE *file;
{
    /*GENERIC *cc = (GENERIC *) ckt;*/

    fprintf(file,"diagnostic output dump unavailable.");
    return;
}

void
if_cktfree(ckt, tab)
    char *ckt;
    char *tab;
{
    GENERIC *cc = (GENERIC *) ckt;

    (*(ft_sim->deleteCircuit))(cc);
    INPtabEnd((INPtables *) tab);
    return;
}

/* Return a string describing an error code. */


/* BLOW THIS AWAY.... */

char *
if_errstring(code)
    int code;
{
    return (INPerror(code));
}

/* Get a parameter value from the circuit. If name is left unspecified,
 * we want a circuit parameter.
 */

struct variable *
if_getparam(ckt, name, param, ind)
    char *ckt;
    char **name;
    char *param;
    int ind;
{
    struct variable *vv = NULL, *tv;
    IFvalue *pv;
    IFparm *opt;
    int typecode, i;
    GENinstance *dev=(GENinstance *)NULL;
    GENmodel *mod=(GENmodel *)NULL;
    IFdevice *device;

    /* fprintf(cp_err, "Calling if_getparam(%s, %s)\n", *name, param); */

    if (param && eq(param, "all")) {
        INPinsert(name,(INPtables *)ft_curckt->ci_symtab);
        typecode = finddev(ckt, *name,(GENERIC**) &dev,(GENERIC**) &mod);
        if (typecode == -1) {
            fprintf(cp_err,
                "Error: no such device or model name %s\n",
                    *name);
            return (NULL);
        }
        device = ft_sim->devices[typecode];
        for (i = 0; i < device->numInstanceParms; i++) {
            opt = &device->instanceParms[i];
            if(!(opt->dataType & IF_ASK)) continue;
            pv = doask(ckt, typecode, dev, mod, opt, ind);
            if (pv) {
                tv = parmtovar(pv, opt);
                if (vv)
                    tv->va_next = vv;
                vv = tv;
            } else
                fprintf(cp_err,
            "Internal Error: no parameter '%s' on device '%s'\n",
                    device->instanceParms[i].keyword,
                    device->name);
        }
        return (vv);
    } else if (param) {
        INPinsert(name,(INPtables *)ft_curckt->ci_symtab);
        typecode = finddev(ckt, *name, (GENERIC**)&dev, (GENERIC**)&mod);
        if (typecode == -1) {
            fprintf(cp_err,
                "Error: no such device or model name %s\n",
                    *name);
            return (NULL);
        }
        device = ft_sim->devices[typecode];
        opt = parmlookup(device, &dev, param);
        if (!opt) {
            fprintf(cp_err, "Error: no such parameter %s.\n",
                    param);
            return (NULL);
        }
        pv = doask(ckt, typecode, dev, mod, opt, ind);
        if (pv)
            vv = parmtovar(pv, opt);
        return (vv);
    } else
        return (if_getstat(ckt, *name));

    /* The way I used to do this was to make @foo refer to all the
     * parameters of the device foo...
     */
#ifdef notdef
    } else if (dev) {
        /* He wants a list of them all. */
        for (i = 0; i < device->DEVquestCount; i++) {
            opt = &device->DEVquests[i];
            pv = doask(ckt, typecode, dev, mod, opt, ind);
            tv = parmtovar(pv, opt);
            if (vv)
                tv->va_next = vv;
            vv = tv;
        }
        return (vv);
    } else {
        for (i = 0; i < device->DEVmodQuestCount; i++) {
            opt = &device->DEVmodQuests[i];
            pv = doask(ckt, typecode, dev, mod, opt, ind);
            tv = parmtovar(pv, opt);
            if (vv)
                tv->va_next = vv;
            vv = tv;
        }
        return (vv);
    }
#endif
}

/* ARGSUSED */
void
if_setparam(ckt, name, param, val)
    char *ckt;
    char *name;
    char *param;
    struct variable *val;
{
}

static struct variable *
parmtovar(pv, opt)
    IFvalue *pv;
    IFparm *opt;
{
    struct variable *vv = alloc(variable);
    struct variable *nv;
    int i = 0;

#ifndef LINT
    switch (opt->dataType & IF_VARTYPES) {
#else
    switch (i) {
#endif
        case IF_INTEGER:
            vv->va_type = VT_NUM;
            vv->va_num = pv->iValue;
            break;
        case IF_REAL:
        case IF_COMPLEX:
            vv->va_type = VT_REAL;
            vv->va_real = pv->rValue;
            break;
        case IF_STRING:
            vv->va_type = VT_STRING;
            vv->va_string = pv->sValue;
            break;
        case IF_FLAG:
            vv->va_type = VT_BOOL;
            vv->va_bool = pv->iValue ? true : false;
            break;
        case IF_REALVEC:
            vv->va_type = VT_LIST;
            for (i = 0; i < pv->v.numValue; i++) {
                nv = alloc(variable);
                nv->va_next = vv->va_vlist;
                vv->va_vlist = nv;
                nv->va_type = VT_REAL;
                nv->va_real = pv->v.vec.rVec[i];
            }
            break;
        default:
            fprintf(cp_err,  
            "parmtovar: Internal Error: bad PARM type %d.\n",
                    opt->dataType);
            return (NULL);
    }

    /* It's not clear whether we want the keyword or the desc here... */
    vv->va_name = copy(opt->description);
    return (vv);
}

/* Extract the IFparm structure from the device. If isdev is true, then get
 * the DEVmodQuest, otherwise get the DEVquest.
 */

static IFparm *
parmlookup(dev, inptr, param)
    IFdevice *dev;
    GENinstance **inptr;
    char *param;
{
    int i;

    /* fprintf(cp_err, "Called: parmlookup(%x, %c, %s)\n", 
            dev, isdev, param); */

    /* First try the device questions... */
    if (inptr) {
        for (i = 0; i < dev->numInstanceParms; i++) {
            if ((dev->instanceParms[i].dataType & IF_ASK) &&
                    eq(dev->instanceParms[i].keyword, param))
                return (&dev->instanceParms[i]);
        }
    }

    /* Zero the device pointer so that CKTmodAsk will get called...
     * This is ugly...
     */
    *inptr = NULL;
    for (i = 0; i < dev->numModelParms; i++)
        if ( (dev->instanceParms[i].dataType & IF_ASK) &&
                eq(dev->modelParms[i].keyword, param))
            return (&dev->modelParms[i]);
    return (NULL);
}

/* Perform the CKTask call. We have both 'fast' and 'modfast', so the other
 * parameters aren't necessary.
 */

/* ARGSUSED */
static IFvalue *
doask(ckt, typecode, dev, mod, opt, ind)
    char *ckt;
    GENinstance *dev;
    GENmodel *mod;
    IFparm *opt;
    int ind;
{
    static IFvalue pv;
    int err;

    pv.iValue = ind;    /* Sometimes this will be junk and ignored... */

    /* fprintf(cp_err, "Calling doask(%d, %x, %x, %x)\n", 
            typecode, dev, mod, opt); */
    if (dev)
        err = (*(ft_sim->askInstanceQuest))((GENERIC *)ckt, (GENERIC *)dev, 
                opt->id, &pv, (IFvalue *)NULL);
    else
        err = (*(ft_sim->askModelQuest))((GENERIC*)ckt, (GENERIC*) mod, 
                opt->id, &pv, (IFvalue *)NULL);
    if (err != OK) {
        ft_sperror(err, "if_getparam");
        return (NULL);
    }

    return (&pv);
}


/* Get pointers to a device, its model, and its type number given the name. If
 * there is no such device, try to find a model with that name.
 */

static int
finddev(ck, name, devptr, modptr)
    char *ck;
    char *name;
    GENERIC **devptr;
    GENERIC **modptr;
{
    int err;
    int type = -1;

    err = (*(ft_sim->findInstance))((GENERIC *)ck,&type,devptr,name,NULL,NULL);
    if(err == OK) return(type);
    type = -1;
    *devptr = (GENERIC *)NULL;
    err = (*(ft_sim->findModel))((GENERIC *)ck,&type,modptr,name);
    if(err == OK) return(type);
    *modptr = (GENERIC *)NULL;
    return(-1);

}

/* Extract the node and device names from the line and add them to the command
 * completion structure.  This is probably not a good thing to do if it
 * takes too much time.
 */

 /* BLOW THIS AWAY */

void
if_setndnames(line)
    char *line;
{
    char *t;
    int i;

/* gtri - add - wbk - 10/23/90 - add new local variables */

    char *next_t;

/* gtri - end - wbk - 10/23/90 */

/* gtri - add - wbk - 10/23/90 - handle new A devices here without */
/* calling inp_numnodes()                                          */

    if((*line == 'a') || (*line == 'A')) {

        /* skip the instance name */
        MIFgettok(&line);

        /* process up to last token */

        next_t = MIFgettok(&line);

        while(1) {

            /* rotate the tokens and get the next one */

            t = next_t;
            next_t = MIFgettok(&line);

            /* if next one is null, current one is model name, */
            /* so exit                                         */

            if(next_t == NULL)
                break;

            /* otherwise, check to see if it is a node name */

            switch(*t) {

            case '{':
            case '}':
            case '~':
                break;

            case '%':
                /* skip over the next token which is the port type */
                next_t = MIFgettok(&line);
                break;

            default:
                /* it's a node name, so add it to the list */
                cp_addkword(CT_DEVNAMES, t);
                break;

            } /*switch */

        } /* while */

        return;

    } /* if */

/* gtri - end - wbk - 10/23/90 */


    while (isspace(*line))
        line++;

    if (!*line || (*line == '*') || (*line == '.'))
        return;
    t = gettok(&line);

    if (!(i = inp_numnodes(*t)))
        return;
    if ((*t == 'q') || (*t == 'Q'))
        i = 3;
    
    cp_addkword(CT_DEVNAMES, t);
    while (i-- > 0) {
        t = gettok(&line);
        if (t)
            cp_addkword(CT_NODENAMES, t);
    }
    return;
}

/* get an analysis parameter by name instead of id */

int if_analQbyName(ckt,which,anal,name,parm)
    GENERIC *ckt;
    int which;
    GENERIC *anal;
    char *name;
    IFvalue *parm;

{
    int i;
    for(i=0;i<ft_sim->analyses[which]->numParms;i++) {
        if(strcmp(ft_sim->analyses[which]->analysisParms[i].keyword,name)==0) {
            return( (*(ft_sim->askAnalysisQuest))(ckt,anal,
                    ft_sim->analyses[which]->analysisParms[i].id,parm,
                    (IFvalue *)NULL) );
        }
    }
    return(E_BADPARM);
}

/* Get the parameters tstart, tstop, and tstep from the CKT struct. */

/* BLOW THIS AWAY TOO */

bool
if_tranparams(ci, start, stop, step)
    struct circ *ci;
    double *start, *stop, *step;
{
    IFvalue tmp;
    int err;
    int which = -1;
    int i;
    GENERIC *anal;
    IFuid tranUid;

    if(!ci->ci_curTask) return(false);
    for(i=0;i<ft_sim->numAnalyses;i++) {
        if(strcmp(ft_sim->analyses[i]->name,"TRAN")==0){
            which = i;
            break;
        }
    }
    if(which == -1) return(false);

    err = IFnewUid(ci->ci_ckt,&tranUid,(IFuid)NULL,"transient",UID_ANALYSIS,
            (GENERIC**)NULL);
    if(err != OK) return(false);
    err =(*(ft_sim->findAnalysis))(ci->ci_ckt,&which, &anal,tranUid,
            ci->ci_curTask,(IFuid *)NULL);
    if(err != OK) return(false);
    err = if_analQbyName(ci->ci_ckt,which,anal,"tstart",&tmp);
    if(err != OK) return(false);
    *start = tmp.rValue;
    err = if_analQbyName(ci->ci_ckt,which,anal,"tstop",&tmp);
    if(err != OK) return(false);
    *stop = tmp.rValue;
    err = if_analQbyName(ci->ci_ckt,which,anal,"tstep",&tmp);
    if(err != OK) return(false);
    *step = tmp.rValue;
    return (true);
}

/* Get the statistic called 'name'.  If this is NULL get all statistics
 * available.
 */

struct variable *
if_getstat(ckt, name)
    char *ckt;
    char *name;
{
    int i;
    struct variable *v, *vars;
    IFvalue parm;
    int which = -1;

    for(i=0;i<ft_sim->numAnalyses;i++) {
        if(strcmp(ft_sim->analyses[i]->name,"options")==0) {
            which = i;
            break;
        }
    }
    if(which==-1) {
        fprintf(cp_err,"Warning:  statistics unsupported\n");
        return(NULL);
    }

    if (name) {
        for (i = 0; i < ft_sim->analyses[which]->numParms; i++)
            if (eq(ft_sim->analyses[which]->analysisParms[i].keyword, name))
                break;
        if (i == ft_sim->analyses[which]->numParms)
            return (NULL);
        if ((*(ft_sim->askAnalysisQuest))(ckt, ft_curckt->ci_curTask,
                ft_sim->analyses[which]->analysisParms[i].id, &parm, 
                (IFvalue *)NULL) == -1) {
            fprintf(cp_err, 
                "if_getstat: Internal Error: can't get %s\n",
                name);
            return (NULL);
        }
        return (parmtovar(&parm, &(ft_sim->analyses[which]->analysisParms[i])));
    } else {
        for (i = 0, vars = v = NULL; i<ft_sim->analyses[which]->numParms; i++) {
            if(!(ft_sim->analyses[which]->analysisParms[i].dataType&IF_ASK)) {
                continue;
            }
            if ((*(ft_sim->askAnalysisQuest))(ckt, ft_curckt->ci_curTask, 
                    ft_sim->analyses[which]->analysisParms[i].id, 
                    &parm, (IFvalue *)NULL) == -1) {
                fprintf(cp_err, 
                "if_getstat: Internal Error: can't get %s\n",
                    name);
                return (NULL);
            }
            if (v) {
                v->va_next = parmtovar(&parm, 
                        &(ft_sim->analyses[which]->analysisParms[i]));
                v = v->va_next;
            } else {
                vars = v = parmtovar(&parm, 
                        &(ft_sim->analyses[which]->analysisParms[i])); 
            }
        }
        return (vars);
    }
}

int SIMinit(frontEnd,simulator)
    IFfrontEnd *frontEnd;
    IFsimulator **simulator;
{
    return(SPIinit(frontEnd,simulator));
}


/* gtri - add - 12/6/90 - wbk - Fake linker into including all obj */
/* modules needed for prelink */
/*
 * Copyright (c) 1987 Thomas L. Quarles
 */

#include "CKTdefs.h"
#include "DEVdefs.h"
#include "JOBdefs.h"
#include "SPerror.h"
#include "OPTdefs.h"

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


SPICEanalysis *dummy_analInfo[] = {
    &OPTinfo,
    &ACinfo,
    &DCTinfo,
    &DCOinfo,
    &TRANinfo,
    &PZinfo,
    &SENinfo,
    &TFinfo,
};

SPICEdev *dummy_DEVices[] = {

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
};


IFsimulator dummy_SIMinfo = {

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

    sizeof(dummy_DEVices)/sizeof(SPICEdev *),
    (IFdevice**) dummy_DEVices,

    sizeof(dummy_analInfo)/sizeof(SPICEanalysis *),
    (IFanalysis **) dummy_analInfo,

    sizeof(nodeParms)/sizeof(IFparm),
    nodeParms,

    sizeof(specSigList)/sizeof(char *),
    specSigList,

};

/* gtri - end - 12/6/90 - wbk */

/* gtri - evt - wbk - 5/20/91 - Add stuff for user-defined nodes */

/* Define dummy struct to force inclusion of idn_digital_info for prelink */

extern Evt_Udn_Info_t idn_digital_info;

Evt_Udn_Info_t  *dummy_evt_udn_info[] = {
    &idn_digital_info,
};

/* gtri - end - wbk - 5/20/91 - Add stuff for user-defined nodes */

