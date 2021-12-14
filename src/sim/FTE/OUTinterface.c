
/* RCS Info: $Revision: 1.7 $ on $Date: 92/08/06 14:39:20 $
 *           $Source: //carob/xspice/src/sim/FTE/RCS/OUTinterface.c,v $
 * Copyright (c) 1988 Wayne A. Christopher, U. C. Berkeley CAD Group 
 *   faustus@cad.berkeley.edu, ucbvax!faustus
 *
 * This module replaces the old "writedata" routines in nutmeg.
 * Unlike the writedata routines, the OUT routines are only called by
 * the simulator routines, and only call routines in nutmeg.  The rest
 * of nutmeg doesn't deal with OUT at all.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "INPdefs.h"        /* for INPtables */
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "version.h"
/* gtri - add - wbk - 12/19/90 - Add IPC stuff */
#include "IPCtiein.h"
#include "MIF.h"
/* gtri - end - wbk - 12/19/90 */
#include "suffix.h"







/******************
 
    These have to go somewhgere else
    */

bool ft_intrpt = false;     /* Set by the (void) signal handlers. */
bool ft_setflag = false;    /* Don't abort after an interrupt. */
static bool shouldstop = false; /* Tell simulator to stop next time it asks. */
static bool printinfo = false;	/* Print informational "error messages". */

extern void gr_end_iplot();







#define DOUBLE_PRECISION    15

typedef struct dataDesc {
    char *name;     /* The name of the vector. */
    int type;       /* The type. */
    bool regular;       /* Is this given to us? */
    int outIndex;       /* If regular then the index. */
    char *specName;     /* The device name if special. */
    char *specParamName;    /* The parameter name if special. */
    int specIndex;      /* For sensitivity, if special. */
    int specType;
    GENERIC *specFast;
    int refIndex;       /* The index of our ref vector. */
    struct dvec *vec;

/* gtri - add - wbk - 4/15/91 - Add member for returning currents to Mspice */
    double specModType;
/* gtri - end - wbk - 4/15/91 - Add member for returning currents to Mspice */

} dataDesc;

typedef struct runDesc {
    GENERIC *analysis;
    GENERIC *circuit;
    char *name;
    char *type;
    int numData;
    int refIndex;
    dataDesc *data;
    bool writeOut;
    bool windowed;
    bool binary;
    struct plot *runPlot;
    FILE *fp;
    long pointPos;          /* where to write pointCount */
    int pointCount;
    int isComplex;
    int windowCount;
} runDesc;

static int beginPlot();
static int addDataDesc();
static int addSpecialDesc();
static void fileInit();
static void fileStartPoint();
static void fileAddRealValue();
static void fileAddComplexValue();
static void fileEndPoint();
static void fileEnd();
static void plotInit();
static void plotAddRealValue();
static void plotAddComplexValue();
static void plotEnd();
static bool parseSpecial();
static bool name_eq();
static bool getSpecial();
static void freeRun();

/* gtri - add - 4/15/91 - wbk - Add prototypes for new functions */
static void addIspecials();
static void addCLDQJMspecials();
/* gtri - add - 4/15/91 - wbk - Add prototypes for new functions */


/* The two "begin plot" routines share all their internals... */

int
OUTpBeginPlot(circuitPtr, analysisPtr, analName, refName, refType,
        numNames, dataNames, dataType, plotPtr)
    GENERIC *circuitPtr;
    GENERIC *analysisPtr;
    IFuid analName;
    IFuid refName;
    int refType;
    int numNames;
    IFuid *dataNames;
    int dataType;
    GENERIC **plotPtr;
{
    char *name;

    if (ft_curckt->ci_ckt == circuitPtr)
      name = ft_curckt->ci_name;
    else
      name = "circuit name";

    return (beginPlot(analysisPtr, circuitPtr, name,
            (char *) analName, (char *) refName, refType, numNames,
            (char **) dataNames, dataType, false,
            (runDesc **) plotPtr));
}

int
OUTwBeginPlot(circuitPtr, analysisPtr, analName, refName, refType,
        numNames, dataNames, dataType, plotPtr)
    GENERIC *circuitPtr;
    GENERIC *analysisPtr;
    IFuid analName;
    IFuid refName;
    int refType;
    int numNames;
    IFuid *dataNames;
    int dataType;
    GENERIC **plotPtr;
{
    return (beginPlot(analysisPtr, circuitPtr, "circuit name",
            (char *) analName, (char *) refName, refType, numNames,
            (char **) dataNames, dataType, true,
            (runDesc **) plotPtr));
}

static int
beginPlot(analysisPtr, circuitPtr, cktName, analName, refName, refType,
        numNames, dataNames, dataType, windowed, runp)
    GENERIC *analysisPtr;
    GENERIC *circuitPtr;
    char *cktName;
    char *analName;
    char *refName;
    int refType;
    int numNames;
    char **dataNames;
    int dataType;
    bool windowed;
    runDesc **runp;
{
    runDesc *run;
    char **saves;
    bool *savesused;
    int numsaves;
    int i, j, depind;
    char namebuf[BSIZE], parambuf[BSIZE], depbuf[BSIZE];
    bool saveall = true;

/* gtri - add - wbk - 12/19/90 - Add IPC stuff */
    char    ipc_mapped_name[BSIZE];
/* gtri - end - wbk - 12/19/90 - Add IPC stuff */


    /* Check to see if we want to print informational data. */
    if (cp_getvar("printinfo", VT_BOOL, (char *) &printinfo))
	fprintf(cp_err, "(debug printing enabled)\n");

    *runp = run = alloc(runDesc);

    /* First fill in some general stuff. */
    run->analysis = analysisPtr;
    run->circuit = circuitPtr;
    run->name = copy(cktName);
    run->type = copy(analName);
    run->windowed = windowed;

/* gtri - add - wbk - 12/19/90 - Add IPC stuff */

    /* Execute this code in lieu of original code if ipc is enabled */

    if(g_ipc.enabled) {

        /* Add the voltage/currents available in the matrix */
        /* to the runDesc structure */

        /* Access in reverse order to make voltage sources come */
        /* out in correct order.  The voltage sources names are */
        /* in an order backwards from the input deck because of */
        /* the way CKTcrtElt adds new instances in the linked   */
        /* lists.  They must be output in the proper order      */
        /* so that the vtrans stuff is correctly received by Mspice */
        for(i = (numNames-1); i >= 0; i--) {
            /* Only send selected items, and map according to #VTRANS */
            if(ipc_screen_name(dataNames[i], ipc_mapped_name))
                addDataDesc(run, ipc_mapped_name, dataType, i);
        }

        /* Add other device currents to the runDesc structure if not AC */
        if(g_ipc.anal_type != IPC_ANAL_AC) {
            /* Current sources */
            addIspecials(run);
            /* Do other devices if returni is enabled */
            if(g_ipc.returni)
                addCLDQJMspecials(run);
        }

        /* Do the stuff that the original routine does at the end */

        run->writeOut = ft_getOutReq(&run->fp, &run->runPlot, &run->binary,
            run->type, run->name);

        if (run->writeOut)
            fileInit(run);
        else
            plotInit(run);

        return (OK);
    }

/* gtri - end - wbk - 12/19/90 */


    /* Now let's see which of these things we need.  First toss in the
     * reference vector.  Then toss in anything that getSaves() tells
     * us to save that we can find in the name list.  Finally unpack
     * the remaining saves into parameters.
     */
    numsaves = ft_getSaves(&saves);
    if (numsaves) {
        savesused = (bool *) tmalloc(sizeof (bool) * numsaves);
        saveall = false;
        for (i = 0; i < numsaves; i++)
            if (cieq(saves[i], "all")) {
                saveall = true;
                break;
            }
    }

    /* Pass 0. */
    if (refName) {
        addDataDesc(run, refName, refType, -1);
        for (i = 0; i < numsaves; i++)
            if (name_eq(saves[i], refName)) {
                savesused[i] = true;
            }
    } else
        run->refIndex = -1;


    /* Pass 1. */
    if (numsaves && !saveall) {
        for (i = 0; i < numsaves; i++) {
            for (j = 0; j < numNames; j++)
                if (name_eq(saves[i], dataNames[j])) {
                    addDataDesc(run, dataNames[j],
                            dataType, j);
                    savesused[i] = true;
                    break;
                }
        }
    } else {
        for (i = 0; i < numNames; i++)
            if (!refName || !name_eq(dataNames[i], refName)) {
                addDataDesc(run, dataNames[i], dataType, i);
            }
    }

    /* Pass 2. */
    for (i = 0; i < numsaves; i++) {
        if (savesused[i])
            continue;
        if (!parseSpecial(saves[i], namebuf, parambuf, depbuf)) {
            fprintf(cp_err, "Warning: can't parse '%s': ignored\n",
                    saves[i]);
            continue;
        }
        /* Now, if there's a dep variable, do we already have it? */
        if (*depbuf) {
            for (j = 0; j < run->numData; j++)
                if (name_eq(depbuf, run->data[j].name))
                    break;
            if (j == run->numData) {
                /* Better add it. */
                for (j = 0; j < numNames; j++)
                    if (name_eq(depbuf, dataNames[j]))
                        break;
                if (j == numNames) {
                    fprintf(cp_err,
            "Warning: can't find '%s': value '%s' ignored\n",
                        depbuf, saves[i]);
                    continue;
                }
                addDataDesc(run, dataNames[j], dataType, j);
                savesused[i] = true;
                depind = j;
            } else
                depind = run->data[j].outIndex;
        }
/* gtri - modify - wbk - 4/15/91 - Modify param list for returning currents to Mspice */
        /* addSpecialDesc(run, saves[i], namebuf, parambuf, depind); */
        addSpecialDesc(run, saves[i], namebuf, parambuf, depind, 1.0);
/* gtri - modify - wbk - 4/15/91 - Modify param list for returning currents to Mspice */
    }

    if (numsaves)
        free(savesused);
    
    /* Now that we have our own data structures built up, let's see what
     * nutmeg wants us to do.
     */
    run->writeOut = ft_getOutReq(&run->fp, &run->runPlot, &run->binary,
            run->type, run->name);

    if (run->writeOut)
        fileInit(run);
    else
        plotInit(run);

    return (OK);
}

static int
addDataDesc(run, name, type, ind)
    runDesc *run;
    char *name;
    int type;
    int ind;
{
    dataDesc *data;

    if (!run->numData)
        run->data = (dataDesc *) tmalloc(sizeof (dataDesc));
    else
        run->data = (dataDesc *) trealloc((char *) run->data,
                sizeof (dataDesc) * (run->numData + 1));
    data = &run->data[run->numData];
    /* so freeRun will get nice NULL pointers for the fields we don't set */
    bzero(data, sizeof(dataDesc));

    data->name = copy(name);
    data->type = type;
    data->regular = true;
    data->outIndex = ind;

    if (ind == -1) {
        /* It's the reference vector. */
        run->refIndex = run->numData;
    }

    run->numData++;

    return (OK);
}

/* gtri - modify - wbk - 4/15/91 - Modify param list for returning currents to Mspice */
/*
static int
addSpecialDesc(run, name, devname, param, depind)
    runDesc *run;
    char *name;
    char *devname;
    char *param;
    int depind;
*/
static int
addSpecialDesc(run, name, devname, param, depind, modtype)
    runDesc *run;
    char *name;
    char *devname;
    char *param;
    int depind;
    double modtype;
/* gtri - modify - wbk - 4/15/91 - Modify param list for returning currents to Mspice */
{
    dataDesc *data;
    char *unique;       /* unique char * from back-end */

    if (!run->numData)
        run->data = (dataDesc *) tmalloc(sizeof (dataDesc));
    else
        run->data = (dataDesc *) trealloc((char *) run->data,
                sizeof (dataDesc) * (run->numData + 1));
    data = &run->data[run->numData];
    /* so freeRun will get nice NULL pointers for the fields we don't set */
    bzero(data, sizeof(dataDesc));

    data->name = copy(name);

/* gtri - modify - wbk - 4/15/91 - Fix Berkeley pointer bug */
/*  The following original Berkeley code frees space pointed to by
    devname, which was never malloced ...
    unique = devname;
    INPinsert(&unique, (INPtables *) ft_curckt->ci_symtab);
    data->specName = unique;
*/
    unique = copy(devname);
    INPinsert(&unique, (INPtables *) ft_curckt->ci_symtab);
    data->specName = unique;
/* gtri - modify - wbk - 4/15/91 - Fix Berkeley pointer bug */

    data->specParamName = copy(param);

    data->specIndex = depind;
    data->specType = -1;
    data->specFast = NULL;
    data->regular = false;

/* gtri - add - wbk - 4/15/91 - Set new model type member for returning currents to Mspice */
    data->specModType = modtype;
/* gtri - end - wbk - 4/15/91 - Set new model type member for returning currents to Mspice */

    run->numData++;

    return (OK);
}



int
OUTpData(plotPtr, refValue, valuePtr)
    GENERIC *plotPtr;
    IFvalue *refValue;
    IFvalue *valuePtr;
{
    runDesc *run = (runDesc *) plotPtr;
    IFvalue val;
    int i;

/* gtri - add - wbk - 12/19/90 - Add IPC stuff */
    Ipc_Complex_t  ipc_ctemp;
/* gtri - add - wbk - 12/19/90 - Add IPC stuff */

    run->pointCount++;

/* gtri - add - wbk - 12/19/90 - Add IPC stuff */

    if(g_ipc.enabled) {

        /* Send ground node data for consistancy */

        if(g_mif_info.circuit.anal_type != MIF_AC) {
            ipc_send_double("0", 0.0);
        }
        else {
            ipc_ctemp.real = 0.0;
            ipc_ctemp.imag = 0.0;
            ipc_send_complex("0", ipc_ctemp);
        }

        /* Send the data from the solution */

        for (i = 0; i < run->numData; i++) {
            if(run->data[i].regular) {
                /* Regular items are from the solution vector */
                /* and include node voltages and Vsource currents */
                if(run->data[i].type == IF_REAL) {
                    ipc_send_double(run->data[i].name,
                        valuePtr->v.vec.rVec[run->data[i].outIndex]);
                }
                else if(run->data[i].type == IF_COMPLEX) {
                    ipc_ctemp.real = valuePtr->v.vec.cVec[run->data[i].outIndex].real;
                    ipc_ctemp.imag = valuePtr->v.vec.cVec[run->data[i].outIndex].imag;
                    ipc_send_complex(run->data[i].name, ipc_ctemp);
                }
            }
            else if(g_ipc.anal_type != IPC_ANAL_AC) {
                /* Must be a special (ICLDQJM current) */
                if(! getSpecial(&run->data[i], run, &val))
                    continue;
                if(run->data[i].type == IF_REAL) {
                    ipc_send_double(run->data[i].specName,
                                    val.rValue * run->data[i].specModType);
                }
            }
        }

        /* Do the stuff this routine does at the end */
        if (ft_bpcheck(run->runPlot, run->pointCount) == false)
            shouldstop = true;

        return (OK);
    }

/* gtri - end - wbk - 12/19/90 - Add IPC stuff */

    if (run->writeOut) {
        fileStartPoint(run->fp, run->binary, run->pointCount);

        if (run->refIndex != -1) {
          if (run->isComplex)
            fileAddComplexValue(run->fp, run->binary, refValue->cValue);
          else
            fileAddRealValue(run->fp, run->binary, refValue->rValue);
        }

        for (i = 0; i < run->numData; i++) {
            /* we've already printed reference vec first */
            if (run->data[i].outIndex == -1) continue;

            if (run->data[i].regular) {
                if(run->data[i].type == IF_REAL)
                  fileAddRealValue(run->fp, run->binary,
                    valuePtr->v.vec.rVec
                    [run->data[i].outIndex]);
                else if (run->data[i].type == IF_COMPLEX)
                  fileAddComplexValue(run->fp, run->binary,
                    valuePtr->v.vec.cVec
                    [run->data[i].outIndex]);
                else
                  fprintf(stderr, "OUTpData: unsupported data type\n");
            } else {
                /* should pre-check instance */
                if (!getSpecial(&run->data[i], run, &val))
                    continue;
                if (run->data[i].type == IF_REAL)
                  fileAddRealValue(run->fp, run->binary,
                     val.rValue);
                else if (run->data[i].type == IF_COMPLEX)
                  fileAddComplexValue(run->fp, run->binary,
                     val.cValue);
                else
                  fprintf(stderr, "OUTpData: unsupported data type\n");
            }
        }
        fileEndPoint(run->fp, run->binary);
    } else {
        for (i = 0; i < run->numData; i++) {
            if (run->data[i].outIndex == -1) {
                if (run->data[i].type == IF_REAL)
                    plotAddRealValue(&run->data[i],
                            refValue->rValue);
                else if (run->data[i].type == IF_COMPLEX)
                    plotAddComplexValue(&run->data[i],
                            refValue->cValue);
            } else if (run->data[i].regular) {
                if (run->data[i].type == IF_REAL)
                    plotAddRealValue(&run->data[i],
                        valuePtr->v.vec.rVec
                        [run->data[i].outIndex]);
                else if (run->data[i].type == IF_COMPLEX)
                    plotAddComplexValue(&run->data[i],
                        valuePtr->v.vec.cVec
                        [run->data[i].outIndex]);
            } else {
                /* should pre-check instance */
                if (!getSpecial(&run->data[i], run, &val))
                    continue;
                if (run->data[i].type == IF_REAL)
                    plotAddRealValue(&run->data[i],
                            val.rValue);
                else if (run->data[i].type == IF_COMPLEX)
                    plotAddComplexValue(&run->data[i],
                            val.cValue);
                else 
                    fprintf(stderr, "OUTpData: unsupported data type\n");
            }
        }
        gr_iplot(run->runPlot);
    }

    if (ft_bpcheck(run->runPlot, run->pointCount) == false)
        shouldstop = true;

    return (OK);
}



/* ARGSUSED */ /* until some code gets written */
int
OUTwReference(plotPtr, valuePtr, refPtr)
    GENERIC *plotPtr;
    IFvalue *valuePtr;
    GENERIC **refPtr;
{
    return (OK);
}

/* ARGSUSED */ /* until some code gets written */
int
OUTwData(plotPtr, dataIndex, valuePtr, refPtr)
    GENERIC *plotPtr;
    int dataIndex;
    IFvalue *valuePtr;
    GENERIC *refPtr;
{
    return (OK);
}

/* ARGSUSED */ /* until some code gets written */
int
OUTwEnd(plotPtr)
    GENERIC *plotPtr;
{
    return (OK);
}



int
OUTendPlot(plotPtr)
    GENERIC *plotPtr;
{
    runDesc *run = (runDesc *) plotPtr;
    
    if (run->writeOut)
        fileEnd(run);
    else {
        gr_end_iplot();
        plotEnd(run);
    }

    freeRun(run);

    return (OK);
}



/* ARGSUSED */ /* until some code gets written */
int
OUTbeginDomain(plotPtr, refName, refType, outerRefValue)
    GENERIC *plotPtr;
    char *refName;
    int refType;
    IFvalue *outerRefValue;
{
    return (OK);
}

/* ARGSUSED */ /* until some code gets written */
int
OUTendDomain(plotPtr)
    GENERIC *plotPtr;
{
    return (OK);
}



/* ARGSUSED */ /* until some code gets written */
int
OUTattributes(plotPtr, varName, param, value)
    GENERIC *plotPtr;
    char *varName;
    int param;
    IFvalue *value;
{
    /*runDesc *run = (runDesc *) plotPtr;*/

    return (OK);
}



/* The file writing routines. */

static void
fileInit(run)
    runDesc *run;
{
    int i;
    char *name, buf[BSIZE];
    int type;

    /* This is a hack. */
    run->isComplex = false;
    for (i = 0; i < run->numData; i++)
        if (run->data[i].type == IF_COMPLEX)
            run->isComplex = true;

    fprintf(run->fp, "Title: %s\n", run->name);
    fprintf(run->fp, "Date: %s\n", datestring());
    fprintf(run->fp, "Plotname: %s\n", run->type);
    fprintf(run->fp, "Flags: %s\n", run->isComplex ? "complex" : "real");
    fprintf(run->fp, "No. Variables: %d\n", run->numData);
    fprintf(run->fp, "No. Points: ");

    fflush(run->fp);        /* Gotta do this for LATTICE. */
    run->pointPos = ftell(run->fp);
    fprintf(run->fp, "0       \n"); /* Save 8 spaces here. */

    fprintf(run->fp, "Command: version %s\n", SPICE_VERSION);
    fprintf(run->fp, "Variables:\n");

    for (i = 0; i < run->numData; i++) {
        if (isdigit(*run->data[i].name)) {
      (void) sprintf(buf, "V(%s)", run->data[i].name);
          name = buf;
        } else {
          name = run->data[i].name;
        }
        if (substring("#branch", name))
            type = SV_CURRENT;
        else if (cieq(name, "time"))
            type = SV_TIME;
        else if (cieq(name, "frequency"))
            type = SV_FREQUENCY;
        else 
            type = SV_VOLTAGE;
        fprintf(run->fp, "\t%d\t%s\t%s\n", i, name,
                ft_typenames(type));
    }

    fprintf(run->fp, "%s:\n", run->binary ? "Binary" : "Values");

    return;
}

static void
fileStartPoint(fp, bin, num)
    FILE *fp;
    bool bin;
    int num;
{
    if (!bin)
        fprintf(fp, "%d\t", num - 1);

    return;
}

static void
fileAddRealValue(fp, bin, value)
    FILE *fp;
    bool bin;
    double value;
{
    if (bin)
        fwrite((char *) &value, sizeof (double), 1, fp);
    else
        fprintf(fp, "\t%.*e\n", DOUBLE_PRECISION, value);
    
    return;
}

static void
fileAddComplexValue(fp, bin, value)
    FILE *fp;
    bool bin;
    IFcomplex value;
{

    if (bin) {
        fwrite((char *) &value.real, sizeof (double), 1, fp);
        fwrite((char *) &value.imag, sizeof (double), 1, fp);
    } else {
        fprintf(fp, "\t%.*e,%.*e\n", DOUBLE_PRECISION, value.real,
                DOUBLE_PRECISION, value.imag);
    }

}

/* ARGSUSED */ /* until some code gets written */
static void
fileEndPoint(fp, bin)
    FILE *fp;
    bool bin;
{
    return;
}

/* Here's the hack...  Run back and fill in the number of points. */

static void
fileEnd(run)
    runDesc *run;
{
    long place;

    fflush(run->fp);    /* For LATTICE... */
    place = ftell(run->fp);
    fseek(run->fp, run->pointPos, 0);
    fprintf(run->fp, "%d", run->pointCount);
    fseek(run->fp, place, 0);
    fflush(run->fp);

    return;
}



/* The plot maintenance routines. */

static void
plotInit(run)
    runDesc *run;
{
    struct plot *pl = plot_alloc(run->type);
    char buf[100];
    struct dvec *v;
    dataDesc *dd;
    int i;

    pl->pl_title = copy(run->name);
    pl->pl_name = copy(run->type);
    pl->pl_date = datestring();
    plot_new(pl);
    plot_setcur(pl->pl_typename);
    run->runPlot = pl;

    /* This is a hack. */
    /* if any of them complex, make them all complex */
    run->isComplex = false;
    for (i = 0; i < run->numData; i++) {
      if (run->data[i].type == IF_COMPLEX) run->isComplex = true;
    }

    for (i = 0; i < run->numData; i++) {
        dd = &run->data[i];
        v = alloc(dvec);
        if (isdigit(*dd->name)) {
            (void) sprintf(buf, "V(%s)", dd->name);
            v->v_name = copy(buf);
        } else
            v->v_name = copy(dd->name);
        if (substring("#branch", v->v_name))
            v->v_type = SV_CURRENT;
        else if (cieq(v->v_name, "time"))
            v->v_type = SV_TIME;
        else if (cieq(v->v_name, "frequency"))
            v->v_type = SV_FREQUENCY;
        else 
            v->v_type = SV_VOLTAGE;
        v->v_length = 0;
        v->v_scale = NULL;
        if (!run->isComplex) {
            v->v_flags = VF_REAL;
            v->v_realdata = (double *) malloc(0);
        } else {
            v->v_flags = VF_COMPLEX;
            v->v_compdata = (complex *) malloc(0);
        }

        v->v_flags |= VF_PERMANENT;

        vec_new(v);
        dd->vec = v;
    }
}

static void
plotAddRealValue(desc, value)
    dataDesc *desc;
    double value;
{
    struct dvec *v = desc->vec;

    if (isreal(v)) {
      v->v_realdata = (double *) realloc((char *) v->v_realdata,
            sizeof (double) * (v->v_length + 1));
      v->v_realdata[v->v_length] = value;
    } else {
      /* a real parading as a VF_COMPLEX */
      v->v_compdata = (complex *) realloc((char *) v->v_compdata,
            sizeof (complex) * (v->v_length + 1));
      v->v_compdata[v->v_length].cx_real = value;
      v->v_compdata[v->v_length].cx_imag = (double) 0;
    }
    v->v_length++;

    return;
}

static void
plotAddComplexValue(desc, value)
    dataDesc *desc;
    IFcomplex value;
{
    struct dvec *v = desc->vec;

    v->v_compdata = (complex *) realloc((char *) v->v_compdata,
            sizeof (complex) * (v->v_length + 1));
    v->v_compdata[v->v_length].cx_real = value.real;
    v->v_compdata[v->v_length].cx_imag = value.imag;
    v->v_length++;

    return;
}

/* ARGSUSED */ /* until some code gets written */
static void
plotEnd(run)
    runDesc *run;
{

    return;
}



/* ParseSpecial takes something of the form "@name[param,index]" and rips
 * out name, param, and index.
 */

static bool
parseSpecial(name, dev, param, ind)
    char *name;
    char *dev;
    char *param;
    char *ind;
{
    char *s;

    *dev = *param = *ind = '\0';

    if (*name != '@')
        return (false);
    name++;
    
    s = dev;
    while (*name && (*name != '['))
        *s++ = *name++;
    *s = '\0';
    if (!*name)
        return (true);
    name++;

    s = param;
    while (*name && (*name != ',') && (*name != ']'))
        *s++ = *name++;
    *s = '\0';
    if (*name == ']')
        return (!name[1] ? true : false);
    else if (!*name)
        return (false);
    name++;

    s = ind;
    while (*name && (*name != ']'))
        *s++ = *name++;
    *s = '\0';
    if (*name && !name[1])
        return (true);
    else
        return (false);
}

/* This routine must match two names with or without a V() around them. */

static bool
name_eq(n1, n2)
    char *n1, *n2;
{
    char buf1[BSIZE], buf2[BSIZE], *s;

    if (s = index(n1, '(')) {
        strcpy(buf1, s);
        if (!(s = index(buf1, ')')))
            return (false);
        *s = '\0';
        n1 = buf1;
    }
    if (s = index(n2, '(')) {
        strcpy(buf2, s);
        if (!(s = index(buf2, ')')))
            return (false);
        *s = '\0';
        n2 = buf2;
    }

    return (strcmp(n1, n2) ? false : true);
}

static bool
getSpecial(desc, run, val)
    dataDesc *desc;
    runDesc *run;
    IFvalue *val;
{
    IFvalue selector;
    struct variable *vv;

    selector.iValue = desc->specIndex;
    if (INPaName(desc->specParamName, val, run->circuit, &desc->specType,
            desc->specName, &desc->specFast, ft_sim, &desc->type,
            &selector) == OK) {
      desc->type &= (IF_REAL | IF_COMPLEX);   /* mask out other bits */
      return(true);
    } else if (vv = if_getstat(run->circuit, &desc->name[1])) {
						/* skip @ sign */
      desc->type = IF_REAL;
      if (vv->va_type == VT_REAL)
	val->rValue = vv->va_real;
      else if (vv->va_type == VT_NUM)
	val->rValue = vv->va_num;
      else if (vv->va_type == VT_BOOL)
	val->rValue = (vv->va_bool ? 1.0 : 0.0);
      else {
	return (false); /* not a real */
      }
      tfree(vv);
      return(true);
    }

    return (false);
}

static void
freeRun(run)
    runDesc *run;
{

    int i;

    for (i=0; i < run->numData; i++) {
/*    vec_free(run->data[i].vec); */ /* kill run, leave plot */
      tfree(run->data[i].name);
      tfree(run->data[i].specParamName);
    }
    tfree(run->data);

/*  killplot(run->runPlot); */ /* kill run, leave plot */

    free(run->type);
    free(run->name);
    free(run);

}

int
OUTstopnow()
{

    if (ft_intrpt || shouldstop) {
        ft_intrpt = shouldstop = false;
        return (1);
    } else
        return (0);

}

/* Print out error messages. */

static struct mesg {
        char *string;
        long flag;
} msgs[] = {
        { "Warning", ERR_WARNING } ,
        { "Fatal error", ERR_FATAL } ,
        { "Panic", ERR_PANIC } ,
        { "Note", ERR_INFO } ,
        { NULL, 0 }
} ;

int OUTerror(flags,format,names) 
    int flags;
    char *format;
    IFuid *names;
{

    struct mesg *m;
    char buf[BSIZE], *s, *bptr;
    int nindex = 0;

    if ((flags == ERR_INFO) && !printinfo)
	return;

    for (m = msgs; m->flag; m++)
                if (flags & m->flag)
                        fprintf(cp_err, "%s: ", m->string);

    for (s = format, bptr = buf; *s; s++) {
      if (*s == '%' && (s == format || *(s-1) != '%') && *(s+1) == 's') {
        strcpy(bptr, names[nindex]);
        bptr += strlen(names[nindex]);
        s++;
        nindex++;
      } else {
        *bptr++ = *s;
      }
    }
    *bptr = '\0';
        fprintf(cp_err, "%s\n", buf);
        fflush(cp_err);

}

#ifdef notdef   /* now converted */
/* This is to make the linker happy while the code is being converted. */

int
wrd_open(filename, title, asc)
    char *filename, *title;
{
    abort();
}

int
wrd_run(title)
    char *title;
{
    abort();
}

int
wrd_init(plotname, flags)
    char *plotname;
    short flags;
{
    abort();
}

int
wrd_command(com)
    char *com;
{
    abort();
}

int
wrd_output(oname, type, flags, extra)
    char *oname;
    int type;
    short flags;
    char *extra;
{
    abort();
}

int
wrd_point(rdata, idata)
    double *rdata, *idata;
{
    abort();
}

int
wrd_end()
{
    abort();
}

int
wrd_close()
{
    abort();
}

void
wrd_chtrace(name, what, which)
    char *name;
    bool what;
{
    abort();
}

void
wrd_version(string)
    char *string;
{
    abort();
}

/* unfortunately, this one has to live after the endif until */
/* the read OUTerror function gets written and INPerror can be converted */
/* ARGSUSED */
void
wrd_error(mess, flags)
    char *mess;
    long flags;
{
    abort();
}

#endif

/* gtri - add - 4/15/91 - wbk - Add new routines for returning currents */

static void addIspecials(run)
runDesc *run;
{
    char **names;

    double *modtypes;

    int  num_names;
    int  i;

    /* Get a list of names containing each I source not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "Isource", &names, &modtypes);

    /* Add the instances to the dataDesc array as special items */
    /* We use the 'dc' parameter since the 'c' parameter is specified */
    /* as input only (IP macro used in its definition) */
    for(i = 0; i < num_names; i++)
        addSpecialDesc(run, "", names[i], "current", 0, modtypes[i]);

    /* Free the space allocated by ipc_get_devices() */
    ipc_free_devices(num_names, names, modtypes);
}


static void addCLDQJMspecials(run)
runDesc *run;
{
    char **names;

    double *modtypes;

    int  num_names;
    int  i;

    /* Add each capacitor not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "Capacitor", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "c", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

    /* Add each inductor not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "Inductor", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "current", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

    /* Add each diode not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "Diode", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "c", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

    /* Add each BJT not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "BJT", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "cc", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cb", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

    /* Add each JFET not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "JFET", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "cd", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cg", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

    /* Add each level 1 MOSFET not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "Mos1", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "cd", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cs", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cb", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

    /* Add each level 2 MOSFET not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "Mos2", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "cd", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cs", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cb", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

    /* Add each level 3 MOSFET not in a subcircuit */
    num_names = ipc_get_devices(run->circuit, "Mos3", &names, &modtypes);
    for(i = 0; i < num_names; i++) {
        addSpecialDesc(run, "", names[i], "cd", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cs", 0, modtypes[i]);
        addSpecialDesc(run, "", names[i], "cb", 0, modtypes[i]);
    }
    ipc_free_devices(num_names, names, modtypes);

}



/* gtri - end - 4/15/91 - wbk - Add new routines for returning currents */

