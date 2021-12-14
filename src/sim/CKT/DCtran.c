/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* subroutine to do DC TRANSIENT analysis    
        --- ONLY, unlike spice2 routine with the same name! */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
/* gtri - add - wbk - Add headers */
#include "MIFtypes.h" 
/* gtri - end - wbk - Add headers */
#include "TRANdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "SPerror.h"

/* gtri - add - wbk - Add headers */
#include "EVT.h"
#include "MIF.h"
#include "EVTproto.h"
#include "IPCtiein.h"
/* gtri - end - wbk - Add headers */

#include "suffix.h"
void bcopy();   /* shouldn't need this, but not defined in a sys. header file */

RCSID("DCtran.c $Revision: 1.14 $ on $Date: 92/06/15 18:44:07 $")

int
DCtran(ckt,restart) 
    CKTcircuit *ckt;
    int restart;    /* forced restart flag */
{
    int i;
    double olddelta;
    double delta;
    double new;
    double *temp;
    double startdTime;
    double startsTime;
    double startTime;
    int startIters;
    int converged;
    int firsttime;
    int error;
#ifdef SENSDEBUG
    FILE *outsen;
#endif /* SENSDEBUG */
    int save_order;
    int save;
    int save2;
    int size;
    long save_mode;
    long save1;
    static char *msg = "Timestep too small";
    IFuid timeUid;
    IFuid *nameList;
    int numNames;
#ifdef SHORTMACRO
    double mt;  /* temporary so macro call won't cross line boundry */
#endif

/* gtri - add - wbk - 12/19/90 - Add IPC stuff */
    Ipc_Boolean_t  ipc_firsttime = IPC_TRUE;
    Ipc_Boolean_t  ipc_secondtime = IPC_FALSE;
    Ipc_Boolean_t  ipc_delta_cut = IPC_FALSE;
    double         ipc_last_time = 0.0;
    double         ipc_last_delta = 0.0;
/* gtri - end - wbk - 12/19/90 - Add IPC stuff */



    if(restart || ckt->CKTtime == 0) {
        delta=MIN(ckt->CKTfinalTime/50,ckt->CKTstep)/10;
        if(ckt->CKTbreaks) FREE(ckt->CKTbreaks);
        ckt->CKTbreaks=(double *)MALLOC(2*sizeof(double));
        if(ckt->CKTbreaks == (double *)NULL) return(E_NOMEM);
        *(ckt->CKTbreaks)=0;
        *(ckt->CKTbreaks+1)=ckt->CKTfinalTime;
        ckt->CKTbreakSize=2;

/* gtri - begin - wbk - 12/19/90 - Modify setting of CKTminBreak */
/*      if(ckt->CKTminBreak==0) ckt->CKTminBreak=ckt->CKTmaxStep*5e-5; */
        /* Set to 10 times delmin for ATESSE 1 compatibity */
        if(ckt->CKTminBreak==0) ckt->CKTminBreak = 10.0 * ckt->CKTdelmin;
/* gtri - end - wbk - 12/19/90 - Modify setting of CKTminBreak */

/* gtri - add - wbk - 12/19/90 - Add IPC stuff and set anal_init and anal_type */

        /* Tell the beginPlot routine what mode we're in */
        g_ipc.anal_type = IPC_ANAL_TRAN;

        /* Tell the code models what mode we're in */
        g_mif_info.circuit.anal_type = MIF_DC;

        g_mif_info.circuit.anal_init = MIF_TRUE;

/* gtri - end - wbk */

        error = CKTnames(ckt,&numNames,&nameList);
        if(error) return(error);
        (*(SPfrontEnd->IFnewUid))((GENERIC *)ckt,&timeUid,(IFuid)NULL,
                "time", UID_OTHER, (GENERIC **)NULL);
        (*(SPfrontEnd->OUTpBeginPlot))((GENERIC *)ckt,(GENERIC*)ckt->CKTcurJob,
                ckt->CKTcurJob->JOBname,timeUid,IF_REAL,numNames,nameList,
                IF_REAL,&(((TRANan*)ckt->CKTcurJob)->TRANplot));

        ckt->CKTtime = 0;
        ckt->CKTdelta = 0;
        ckt->CKTbreak=1;
        firsttime = 1;
        save_mode = (ckt->CKTmode&MODEUIC)|MODETRANOP | MODEINITJCT;
        save_order = ckt->CKTorder;

/* gtri - begin - wbk - set a breakpoint at end of supply ramping time */
        /* must do this after CKTtime set to 0 above */
        if(ckt->enh->ramp.ramptime > 0.0)
            CKTsetBreak(ckt, ckt->enh->ramp.ramptime);
/* gtri - end - wbk - set a breakpoint at end of supply ramping time */

/* gtri - begin - wbk - Call EVTop if event-driven instances exist */
    if(ckt->evt->counts.num_insts == 0) {
        /* If no event-driven instances, do what SPICE normally does */
        converged = CKTop(ckt,
                (ckt->CKTmode & MODEUIC)|MODETRANOP| MODEINITJCT,
                (ckt->CKTmode & MODEUIC)|MODETRANOP| MODEINITFLOAT,
                ckt->CKTdcMaxIter);
        if(converged != 0) return(converged);
    }
    else {
        /* Else, use new DCOP algorithm */
        converged = EVTop(ckt,
                (ckt->CKTmode & MODEUIC) | MODETRANOP | MODEINITJCT,
                (ckt->CKTmode & MODEUIC)|MODETRANOP| MODEINITFLOAT,
                ckt->CKTdcMaxIter,
                MIF_TRUE);
        EVTdump(ckt, IPC_ANAL_DCOP, 0.0);
        EVTop_save(ckt, MIF_FALSE, 0.0);
        if(converged != 0)
            return(converged);
    }
/* gtri - end - wbk - Call EVTop if event-driven instances exist */

/* gtri - add - wbk - 12/19/90 - Add IPC stuff */

        /* Send the operating point results for Mspice compatibility */
        if(g_ipc.enabled) {
            ipc_send_dcop_prefix();
            CKTdump(ckt,(double)0,(((TRANan*)ckt->CKTcurJob)->TRANplot));
            ipc_send_dcop_suffix();
        }

/* gtri - end - wbk */


/* gtri - add - wbk - 12/19/90 - set anal_init and anal_type */

        g_mif_info.circuit.anal_init = MIF_TRUE;

        /* Tell the code models what mode we're in */
        g_mif_info.circuit.anal_type = MIF_TRAN;

/* gtri - end - wbk */

/* gtri - begin - wbk - Add Breakpoint stuff */

        /* Initialize the temporary breakpoint variables to infinity */
        g_mif_info.breakpoint.current = 1.0e30;
        g_mif_info.breakpoint.last    = 1.0e30;

/* gtri - end - wbk - Add Breakpoint stuff */

        ckt->CKTstat->STATtimePts ++;
        ckt->CKTorder=1;
        for(i=0;i<7;i++) {
            ckt->CKTdeltaOld[i]=ckt->CKTmaxStep;
        }
        ckt->CKTdelta = delta;
#ifdef STEPDEBUG
        (void)printf("delta initialized to %g\n",ckt->CKTdelta);
#endif
        ckt->CKTsaveDelta = ckt->CKTfinalTime/50;

        if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN)){
#ifdef SENSDEBUG
            printf("\nTransient Sensitivity Results\n\n");
            CKTsenPrint(ckt);
#endif /* SENSDEBUG */
            save = ckt->CKTsenInfo->SENmode;
            ckt->CKTsenInfo->SENmode = TRANSEN;
            save1 = ckt->CKTmode;
            save2 = ckt->CKTorder;
            ckt->CKTmode = save_mode;
            ckt->CKTorder = save_order;
            if(error = CKTsenDCtran(ckt)) return(error);
            ckt->CKTmode = save1;
            ckt->CKTorder = save2;
        }

        ckt->CKTmode = (ckt->CKTmode&MODEUIC)|MODETRAN | MODEINITTRAN;
        ckt->CKTag[0]=ckt->CKTag[1]=0;
        bcopy((char *)ckt->CKTstate0,(char *)ckt->CKTstate1,
                ckt->CKTnumStates*sizeof(double));

        if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN)){
            size = SMPmatSize(ckt->CKTmatrix);
            for(i = 1; i<=size ; i++)
                *(ckt->CKTrhsOp + i) = *(ckt->CKTrhsOld + i);
        }

        startTime=(*(SPfrontEnd->IFseconds))();
        startIters = ckt->CKTstat->STATnumIter;
        startdTime = ckt->CKTstat->STATdecompTime;
        startsTime = ckt->CKTstat->STATsolveTime;
    } else {
        startTime=(*(SPfrontEnd->IFseconds))();
        startIters = ckt->CKTstat->STATnumIter;
        startdTime = ckt->CKTstat->STATdecompTime;
        startsTime = ckt->CKTstat->STATsolveTime;
        firsttime=0;
        goto resume;
    }

/* 650 */
nextTime:
    error = CKTaccept(ckt);
#ifdef STEPDEBUG
    printf("accepted at %g\n",ckt->CKTtime);
#endif /* STEPDEBUG */
    ckt->CKTstat->STATaccepted ++;
    ckt->CKTbreak=0;
    if(error)  {
        ckt->CKTcurrentAnalysis = DOING_TRAN;
        ckt->CKTstat->STATtranTime += (*(SPfrontEnd->IFseconds))()-startTime;
        ckt->CKTstat->STATtranIter += ckt->CKTstat->STATnumIter - startIters;
        ckt->CKTstat->STATtranDecompTime += ckt->CKTstat->STATdecompTime -
                startdTime;
        ckt->CKTstat->STATtranSolveTime += ckt->CKTstat->STATsolveTime -
                startsTime;
        return(error);
    }

/* gtri - modify - wbk - 12/19/90 - Send IPC stuff */

    if(g_ipc.enabled) {

        /* Send event-driven results */
        EVTdump(ckt, IPC_ANAL_TRAN, 0.0);

        /* Then follow with analog results... */

        /* Test to see if delta was cut by a breakpoint, */
        /* a non-convergence, or a too large truncation error */
        if(ipc_firsttime)
            ipc_delta_cut = IPC_FALSE;
        else if(ckt->CKTtime < (ipc_last_time + (0.999 * ipc_last_delta)))
            ipc_delta_cut = IPC_TRUE;
        else
            ipc_delta_cut = IPC_FALSE;

        /* Record the data required to check for delta cuts */
        ipc_last_time = ckt->CKTtime;
        ipc_last_delta = MIN(ckt->CKTdelta, ckt->CKTmaxStep);

        /* Send results data if time since last dump is greater */
        /* than 'mintime', or if first or second timepoints, */
        /* or if delta was cut */
        if( (ckt->CKTtime >= (g_ipc.mintime + g_ipc.last_time)) ||
            ipc_firsttime || ipc_secondtime || ipc_delta_cut ) {

            ipc_send_data_prefix(ckt->CKTtime);
            CKTdump(ckt,ckt->CKTtime,
                  (((TRANan*)ckt->CKTcurJob)->TRANplot));
            ipc_send_data_suffix();

            if(ipc_firsttime) {
                ipc_firsttime = IPC_FALSE;
                ipc_secondtime = IPC_TRUE;
            }
            else if(ipc_secondtime)
                ipc_secondtime = IPC_FALSE;

            g_ipc.last_time = ckt->CKTtime;
        }
    }
    else {
        /* Original code */
        if(ckt->CKTtime >= ckt->CKTinitTime) {
            CKTdump(ckt,ckt->CKTtime,
                 (((TRANan*)ckt->CKTcurJob)->TRANplot));
        }
    }

/* gtri - modify - wbk - 12/19/90 - Send IPC stuff */

/* gtri - begin - wbk - Update event queues/data for accepted timepoint */

    /* Note: this must be done AFTER sending results to SI so it can't */
    /* go next to CKTaccept() above */
    if(ckt->evt->counts.num_insts > 0)
        EVTaccept(ckt, ckt->CKTtime);

/* gtri - end - wbk - Update event queues/data for accepted timepoint */

    ckt->CKTstat->STAToldIter = ckt->CKTstat->STATnumIter;
    if(FABS(ckt->CKTtime - ckt->CKTfinalTime) < ckt->CKTminBreak) {
        /*printf(" done:  time is %g, final time is %g, and tol is %g\n",*/
        /*ckt->CKTtime,ckt->CKTfinalTime,ckt->CKTminBreak);*/
        (*(SPfrontEnd->OUTendPlot))( (((TRANan*)ckt->CKTcurJob)->TRANplot));
        ckt->CKTcurrentAnalysis = 0;
        ckt->CKTstat->STATtranTime += (*(SPfrontEnd->IFseconds))()-startTime;
        ckt->CKTstat->STATtranIter += ckt->CKTstat->STATnumIter - startIters;
        ckt->CKTstat->STATtranDecompTime += ckt->CKTstat->STATdecompTime -
                startdTime;
        ckt->CKTstat->STATtranSolveTime += ckt->CKTstat->STATsolveTime -
                startsTime;
        if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN)){
            ckt->CKTsenInfo->SENmode = save;
#ifdef SENSDEBUG
            fclose(outsen);
#endif /* SENSDEBUG */
        }
        return(OK);
    }
    if( (*(SPfrontEnd->IFpauseTest))() ) {
        /* user requested pause... */
        ckt->CKTcurrentAnalysis = DOING_TRAN;
        ckt->CKTstat->STATtranTime += (*(SPfrontEnd->IFseconds))()-startTime;
        ckt->CKTstat->STATtranIter += ckt->CKTstat->STATnumIter - startIters;
        ckt->CKTstat->STATtranDecompTime += ckt->CKTstat->STATdecompTime -
                startdTime;
        ckt->CKTstat->STATtranSolveTime += ckt->CKTstat->STATsolveTime -
                startsTime;
        return(E_PAUSE);
    }
resume:
#ifdef STEPDEBUG
    if( (ckt->CKTdelta <= ckt->CKTfinalTime/50) && 
            (ckt->CKTdelta <= ckt->CKTmaxStep)) {
            ;
    } else {
        if(ckt->CKTfinalTime/50<ckt->CKTmaxStep) {
            (void)printf("limited by Tstop/50\n");
        } else {
            (void)printf("limited by Tmax\n");
        }
    }
#endif
    ckt->CKTdelta = 
            MIN(ckt->CKTdelta,ckt->CKTmaxStep);

/* gtri - begin - wbk - Cut integration order if first timepoint after breakpoint */
    if(ckt->CKTtime == g_mif_info.breakpoint.last)
        ckt->CKTorder = 1;
/* gtri - end   - wbk - Cut integration order if first timepoint after breakpoint */


    if(ckt->CKTtime == *(ckt->CKTbreaks)) {
        /* first timepoint after a breakpoint - cut integration order */
        /* and limit timestep to .1 times minimum of time to next breakpoint,
         *  and previous timestep
         */
        ckt->CKTorder = 1;
#ifdef STEPDEBUG
        if( (ckt->CKTdelta >.1* ckt->CKTsaveDelta) ||
                (ckt->CKTdelta > .1*(*(ckt->CKTbreaks+1)-*(ckt->CKTbreaks))) ) {
            if(ckt->CKTsaveDelta < (*(ckt->CKTbreaks+1)-*(ckt->CKTbreaks)))  {
                (void)printf("limited by pre-breakpoint delta\n");
            } else {
                (void)printf("limited by next breakpoint\n");
            }
        }
#endif
#ifdef SHORTMACRO
            mt= MIN(ckt->CKTsaveDelta, *(ckt->CKTbreaks+1)-*(ckt->CKTbreaks));
            ckt->CKTdelta = MIN(ckt->CKTdelta, .1 * mt);
#else
            ckt->CKTdelta = MIN(ckt->CKTdelta, .1 * MIN(ckt->CKTsaveDelta,
                    *(ckt->CKTbreaks+1)-*(ckt->CKTbreaks)));
#endif
        if(firsttime) {
            ckt->CKTdelta /= 10;
#ifdef STEPDEBUG
            (void)printf("delta cut for initial timepoint\n");
#endif
        } 
    }

/* gtri - begin - wbk - Add Breakpoint stuff */

    if(ckt->CKTtime + ckt->CKTdelta >= g_mif_info.breakpoint.current) {
        /* If next time > temporary breakpoint, force it to the breakpoint */
        /* And mark that timestep was set by temporary breakpoint */
        ckt->CKTsaveDelta = ckt->CKTdelta;
        ckt->CKTdelta = g_mif_info.breakpoint.current - ckt->CKTtime;
        g_mif_info.breakpoint.last = ckt->CKTtime + ckt->CKTdelta;
    }
    else {
        /* Else, mark that timestep was not set by temporary breakpoint */
        g_mif_info.breakpoint.last = 1.0e30;
    }

/* gtri - end - wbk - Add Breakpoint stuff */

/* gtri - begin - wbk - Modify Breakpoint stuff */


/* Old Code:

    if(ckt->CKTtime + ckt->CKTdelta >= *(ckt->CKTbreaks+1)) {
        ckt->CKTsaveDelta = ckt->CKTdelta;
        ckt->CKTdelta = *(ckt->CKTbreaks+1) - ckt->CKTtime;
#ifdef STEPDEBUG
        (void)printf("delta cut to hit breakpoint\n");
#endif
        ckt->CKTbreak = 1;
        error = CKTclrBreak(ckt);
        if(error) {
            ckt->CKTcurrentAnalysis = DOING_TRAN;
            ckt->CKTstat->STATtranTime += 
                    (*(SPfrontEnd->IFseconds))()-startTime;
            ckt->CKTstat->STATtranIter += 
                    ckt->CKTstat->STATnumIter - startIters;
            ckt->CKTstat->STATtranDecompTime += ckt->CKTstat->STATdecompTime -
                    startdTime;
            ckt->CKTstat->STATtranSolveTime += ckt->CKTstat->STATsolveTime -
                    startsTime;
            return(error);
        }
    } else if((ckt->CKTtime + ckt->CKTdelta > *(ckt->CKTbreaks)) &&
            (ckt->CKTtime < *(ckt->CKTbreaks))) {
        ckt->CKTbreak = 1;
        ckt->CKTsaveDelta = ckt->CKTdelta;
#ifdef STEPDEBUG
        (void)printf("delta cut to hit breakpoint again\n");
#endif
        ckt->CKTdelta = *(ckt->CKTbreaks) - ckt->CKTtime;
    }

    New Code:
*/

    /* Throw out any permanent breakpoint times <= current time */
    while(1) {
        if(*(ckt->CKTbreaks) <= (ckt->CKTtime + ckt->CKTminBreak))
            CKTclrBreak(ckt);
        else
            break;
    }
    /* Force the breakpoint if appropriate */
    if((ckt->CKTtime + ckt->CKTdelta) > *(ckt->CKTbreaks)) {
        ckt->CKTbreak = 1;
        ckt->CKTsaveDelta = ckt->CKTdelta;
        ckt->CKTdelta = *(ckt->CKTbreaks) - ckt->CKTtime;
    }

/* gtri - end - wbk - Modify Breakpoint stuff */


/* gtri - begin - wbk - Do event solution */

    if(ckt->evt->counts.num_insts > 0) {

        /* if time = 0 and op_alternate was specified as false during */
        /* dcop analysis, call any changed instances to let them */
        /* post their outputs with their associated delays */
        if((ckt->CKTtime == 0.0) && (! ckt->evt->options.op_alternate))
            EVTiter(ckt);

        /* while there are events on the queue with event time <= next */
        /* projected analog time, process them */
        while((g_mif_info.circuit.evt_step = EVTnext_time(ckt))
               <= (ckt->CKTtime + ckt->CKTdelta)) {

            /* Initialize temp analog bkpt to infinity */
            g_mif_info.breakpoint.current = 1e30;

            /* Pull items off queue and process them */
            EVTdequeue(ckt, g_mif_info.circuit.evt_step);
            EVTiter(ckt);

            /* If any instances have forced an earlier */
            /* next analog time, cut the delta */
            if(*(ckt->CKTbreaks) < g_mif_info.breakpoint.current)
                if(*(ckt->CKTbreaks) > (ckt->CKTtime + ckt->CKTminBreak))
                    g_mif_info.breakpoint.current = *(ckt->CKTbreaks);
            if(g_mif_info.breakpoint.current < (ckt->CKTtime + ckt->CKTdelta)) {
                /* Breakpoint must be > last accepted timepoint */
                /* and >= current event time */
                if(g_mif_info.breakpoint.current > (ckt->CKTtime + ckt->CKTminBreak)
                   && (g_mif_info.breakpoint.current >= g_mif_info.circuit.evt_step)) {
                    ckt->CKTsaveDelta = ckt->CKTdelta;
                    ckt->CKTdelta = g_mif_info.breakpoint.current - ckt->CKTtime;
                    g_mif_info.breakpoint.last = ckt->CKTtime + ckt->CKTdelta;
                }
            }

        } /* end while next event time <= next analog time */
    } /* end if there are event instances */

/* gtri - end - wbk - Do event solution */


    for(i=5;i>=0;i--) {
        ckt->CKTdeltaOld[i+1]=ckt->CKTdeltaOld[i];
    }
    ckt->CKTdeltaOld[0]=ckt->CKTdelta;

    temp = ckt->CKTstates[ckt->CKTmaxOrder+1];
    for(i=ckt->CKTmaxOrder;i>=0;i--) {
        ckt->CKTstates[i+1] = ckt->CKTstates[i];
    }
    ckt->CKTstates[0] = temp;

/* 600 */
    while (1) {

/* gtri - add - wbk - 4/17/91 - Fix Berkeley bug */
/* This is needed here to allow CAPask to output currents */
/* during Transient analysis.  A grep for CKTcurrentAnalysis */
/* indicates that it should not hurt anything else ... */

        ckt->CKTcurrentAnalysis = DOING_TRAN;

/* gtri - end - wbk - 4/17/91 - Fix Berkeley bug */

        olddelta=ckt->CKTdelta;
        /* time abort? */
        ckt->CKTtime += ckt->CKTdelta;
        ckt->CKTdeltaOld[0]=ckt->CKTdelta;
        NIcomCof(ckt);
#ifdef PREDICTOR
        error = NIpred(ckt);
#endif /* PREDICTOR */
        save_mode = ckt->CKTmode;
        save_order = ckt->CKTorder;

/* gtri - begin - wbk - Add Breakpoint stuff */

        /* Initialize temporary breakpoint to infinity */
        g_mif_info.breakpoint.current = 1.0e30;

/* gtri - end - wbk - Add Breakpoint stuff */

/* gtri - begin - wbk - add convergence problem reporting flags */
        /* delta is forced to equal delmin on last attempt near line 650 */
        if(ckt->CKTdelta <= ckt->CKTdelmin)
            ckt->enh->conv_debug.last_NIiter_call = MIF_TRUE;
        else
            ckt->enh->conv_debug.last_NIiter_call = MIF_FALSE;
/* gtri - begin - wbk - add convergence problem reporting flags */

/* gtri - begin - wbk - Call all hybrids */

/* gtri - begin - wbk - Set evt_step */

        if(ckt->evt->counts.num_insts > 0) {
            g_mif_info.circuit.evt_step = ckt->CKTtime;
        }

/* gtri - end - wbk - Set evt_step */

        converged = NIiter(ckt,ckt->CKTtranMaxIter);

        if(ckt->evt->counts.num_insts > 0) {
            g_mif_info.circuit.evt_step = ckt->CKTtime;
            EVTcall_hybrids(ckt);
        }

/* gtri - end - wbk - Call all hybrids */

        ckt->CKTstat->STATtimePts ++;
        ckt->CKTmode = (ckt->CKTmode&MODEUIC)|MODETRAN | MODEINITPRED;
        if(firsttime) {
            for(i=0;i<ckt->CKTnumStates;i++) {
                *(ckt->CKTstate2+i) = *(ckt->CKTstate1+i);
                *(ckt->CKTstate3+i) = *(ckt->CKTstate1+i);
            }
        }
        if(converged != 0) {
            ckt->CKTtime = ckt->CKTtime -ckt->CKTdelta;
            ckt->CKTstat->STATrejected ++;
            ckt->CKTdelta = ckt->CKTdelta/8;
#ifdef STEPDEBUG
            (void)printf("delta cut for non-convergence\n");
#endif
            if(firsttime) {
                ckt->CKTmode = (ckt->CKTmode&MODEUIC)|MODETRAN | MODEINITTRAN;
            }
            ckt->CKTorder = 1;

/* gtri - begin - wbk - Add Breakpoint stuff */

        /* Force backup if temporary breakpoint is < current time */
        } else if(g_mif_info.breakpoint.current < ckt->CKTtime) {
            ckt->CKTsaveDelta = ckt->CKTdelta;
            ckt->CKTtime -= ckt->CKTdelta;
            ckt->CKTdelta = g_mif_info.breakpoint.current - ckt->CKTtime;
            g_mif_info.breakpoint.last = ckt->CKTtime + ckt->CKTdelta;
            if(firsttime) {
                ckt->CKTmode = (ckt->CKTmode&MODEUIC)|MODETRAN | MODEINITTRAN;
            }
            ckt->CKTorder = 1;

/* gtri - end - wbk - Add Breakpoint stuff */

        } else {
            if (firsttime) {
                if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN)){
                    save1 = ckt->CKTmode;
                    save2 = ckt->CKTorder;
                    ckt->CKTmode = save_mode;
                    ckt->CKTorder = save_order;
                    if(error = CKTsenDCtran(ckt)) return(error);
                    ckt->CKTmode = save1;
                    ckt->CKTorder = save2;
                }
                firsttime =0;
                goto nextTime;  /* no check on
                                 * first time point
                                 */
            }
            new = ckt->CKTdelta;
            error = CKTtrunc(ckt,&new);
            if(error) {
                ckt->CKTcurrentAnalysis = DOING_TRAN;
                ckt->CKTstat->STATtranTime += 
                        (*(SPfrontEnd->IFseconds))()-startTime;
                ckt->CKTstat->STATtranIter += 
                        ckt->CKTstat->STATnumIter - startIters;
                ckt->CKTstat->STATtranDecompTime += ckt->CKTstat->STATdecompTime
                        - startdTime;
                ckt->CKTstat->STATtranSolveTime += ckt->CKTstat->STATsolveTime
                        - startsTime;
                return(error);
            }
            if(new>.9 * ckt->CKTdelta) {
                if(ckt->CKTorder == 1) {
                    new = ckt->CKTdelta;
                    ckt->CKTorder = 2;
                    error = CKTtrunc(ckt,&new);
                    if(error) {
                        ckt->CKTcurrentAnalysis = DOING_TRAN;
                        ckt->CKTstat->STATtranTime += 
                                (*(SPfrontEnd->IFseconds))()-startTime;
                        ckt->CKTstat->STATtranIter += 
                                ckt->CKTstat->STATnumIter - startIters;
                        ckt->CKTstat->STATtranDecompTime += 
                                ckt->CKTstat->STATdecompTime - startdTime;
                        ckt->CKTstat->STATtranSolveTime += 
                                ckt->CKTstat->STATsolveTime - startsTime;
                        return(error);
                    }
                    if(new <= 1.05 * ckt->CKTdelta) {
                        ckt->CKTorder = 1;
                    }
                }
                /* time point OK  - 630*/
                ckt->CKTdelta = new;
#ifdef STEPDEBUG
                (void)printf(
                    "delta set to truncation error result:point accepted\n");
#endif
                if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN)){
                    save1 = ckt->CKTmode;
                    save2 = ckt->CKTorder;
                    ckt->CKTmode = save_mode;
                    ckt->CKTorder = save_order;
                    if(error = CKTsenDCtran(ckt)) return(error);
                    ckt->CKTmode = save1;
                    ckt->CKTorder = save2;
                }
                /* go to 650 - trapezoidal */
                goto nextTime;
            } else {
                ckt->CKTtime = ckt->CKTtime -ckt->CKTdelta;
                ckt->CKTstat->STATrejected ++;
                ckt->CKTdelta = new;
#ifdef STEPDEBUG
                (void)printf(
                    "delta set to truncation error result:point rejected\n");
#endif
            }
        }
        if (ckt->CKTdelta <= ckt->CKTdelmin) {
            if (olddelta > ckt->CKTdelmin) {
                ckt->CKTdelta = ckt->CKTdelmin;
#ifdef STEPDEBUG
                (void)printf("delta at delmin\n");
#endif
            } else {
                ckt->CKTcurrentAnalysis = DOING_TRAN;
                ckt->CKTstat->STATtranTime += 
                        (*(SPfrontEnd->IFseconds))()-startTime;
                ckt->CKTstat->STATtranIter += 
                        ckt->CKTstat->STATnumIter - startIters;
                ckt->CKTstat->STATtranDecompTime += 
                        ckt->CKTstat->STATdecompTime - startdTime;
                ckt->CKTstat->STATtranSolveTime += 
                        ckt->CKTstat->STATsolveTime - startsTime;
                errMsg = MALLOC(strlen(msg)+1);
                strcpy(errMsg,msg);
                return(E_TIMESTEP);
            }
        }
/* gtri - begin - wbk - Do event backup */

        if(ckt->evt->counts.num_insts > 0)
            EVTbackup(ckt, ckt->CKTtime + ckt->CKTdelta);

/* gtri - end - wbk - Do event backup */
    }
    /* NOTREACHED */
}
