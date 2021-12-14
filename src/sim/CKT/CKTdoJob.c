/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include "CKTdefs.h"
#include <stdio.h>
#include "SPerror.h"
#include "TRANdefs.h"
#include "util.h"

/* gtri - add - wbk - 11/26/90 - add include for MIF and EVT global data */
#include "MIF.h"
#include "EVTproto.h"
/* gtri - end - wbk - 11/26/90 */

/* gtri - add - 12/12/90 - wbk - include ipc stuff */
#include "IPCtiein.h"
/* gtri - end - 12/12/90 */

#include "suffix.h"

RCSID("CKTdoJob.c $Revision: 1.3 $ on $Date: 91/12/19 16:58:49 $")

extern SPICEanalysis *analInfo[];
extern int ANALmaxnum;
int
CKTdoJob(inCkt,reset,inTask)
    GENERIC *inCkt;
    int reset;
    GENERIC *inTask;
{
    register CKTcircuit *ckt = (CKTcircuit *)inCkt;
    register TSKtask *task = (TSKtask *)inTask;
    register JOB *job;
    int error;
    int which;
    int i;
    double startTime;
    unsigned  senflag ;

    startTime = (*(SPfrontEnd->IFseconds))();
    ckt->CKTtemp  = task->TSKtemp;
    ckt->CKTnomTemp  = task->TSKnomTemp;
    ckt->CKTmaxOrder  = task->TSKmaxOrder;
    ckt->CKTintegrateMethod  = task->TSKintegrateMethod;
    ckt->CKTbypass  = task->TSKbypass;
    ckt->CKTdcMaxIter  = task->TSKdcMaxIter;
    ckt->CKTdcTrcvMaxIter  = task->TSKdcTrcvMaxIter;
    ckt->CKTtranMaxIter  = task->TSKtranMaxIter;
    ckt->CKTnumSrcSteps  = task->TSKnumSrcSteps;
    ckt->CKTnumGminSteps  = task->TSKnumGminSteps;
    ckt->CKTminBreak  = task->TSKminBreak;
    ckt->CKTabstol  = task->TSKabstol;
    ckt->CKTpivotAbsTol  = task->TSKpivotAbsTol;
    ckt->CKTpivotRelTol  = task->TSKpivotRelTol;
#ifdef NEWTRUNC
    ckt->CKTlteReltol = task->TSKlteReltol;
    ckt->CKTlteAbstol = task->TSKlteAbstol;
#endif /* NEWTRUNC */
    ckt->CKTreltol  = task->TSKreltol;
    ckt->CKTchgtol  = task->TSKchgtol;
    ckt->CKTvoltTol  = task->TSKvoltTol;
    ckt->CKTgmin  = task->TSKgmin;
    ckt->CKTdelmin  = task->TSKdelmin;
    ckt->CKTtrtol  = task->TSKtrtol;
    ckt->CKTdefaultMosL  = task->TSKdefaultMosL;
    ckt->CKTdefaultMosW  = task->TSKdefaultMosW;
    ckt->CKTdefaultMosAD  = task->TSKdefaultMosAD;
    ckt->CKTdefaultMosAS  = task->TSKdefaultMosAS;
    ckt->CKTfixLimit  = task->TSKfixLimit;
    ckt->CKTnoOpIter  = task->TSKnoOpIter;
    if(reset) {
        which = -1;
        senflag = 0;
        for(i=0;i<ANALmaxnum;i++) {
            if(strcmp("SEN",analInfo[i]->public.name)==0) {
                which = i;
                break;
            }
        }
        if(which != -1) {
            for(job=task->jobs;job;job=job->JOBnextJob) {
                if(job->JOBtype == which) {
                    senflag = 1;
                    ckt->CKTcurJob=job;

                    ckt->CKTsenInfo = (SENstruct *)job;
                    error = SENstartup(ckt);
                    if(error) {
                        ckt->CKTstat->STATtotAnalTime += 
                                (*(SPfrontEnd->IFseconds))()-startTime;
                        /* gtri - add - 12/12/90 - wbk - set ipc syntax error flag */
                        g_ipc.syntax_error = IPC_TRUE;
                        /* gtri - end - 12/12/90 */
                        return(error);
                    }
                }
            }
            if(ckt->CKTsenInfo && !senflag){
                FREE(ckt->CKTsenInfo);
            }
         
        }
        error = CKTsetup(ckt);
        if(error) {
            ckt->CKTstat->STATtotAnalTime += 
                    (*(SPfrontEnd->IFseconds))()-startTime;
            /* gtri - add - 12/12/90 - wbk - set ipc syntax error flag */
            g_ipc.syntax_error = IPC_TRUE;
            /* gtri - end - 12/12/90 */
            return(error);
        }
        error = CKTtemp(ckt);
        if(error) {
            ckt->CKTstat->STATtotAnalTime += 
                    (*(SPfrontEnd->IFseconds))()-startTime;
            /* gtri - add - 12/12/90 - wbk - set ipc syntax error flag */
            g_ipc.syntax_error = IPC_TRUE;
            /* gtri - end - 12/12/90 */
            return(error);
        }
        ckt->CKTcurrentAnalysis = 0;
    }

    /* gtri - add - 12/12/90 - wbk - return if syntax errors from parsing */
    if(g_ipc.enabled) {
        if(g_ipc.syntax_error)
            return(E_PRIVATE);
        else {
            /* else, send (GO) errchk status if we got this far */
            /* Caller is responsible for sending NOGO status if we returned earlier */
            ipc_send_errchk();
        }
    }
    /* gtri - end - 12/12/90 */


    which = -1;
    for(i=0;i<ANALmaxnum;i++) {
        if(strcmp("DCOP",analInfo[i]->public.name)==0) {
            which = i;
            break;
        }
    }
    if(which != -1) {
        for(job=task->jobs;job;job=job->JOBnextJob) {
            if(job->JOBtype == which) {
                ckt->CKTcurJob=job;
                error = CKTic(ckt);
/* gtri - begin - 6/10/91 - wbk - Setup event-driven data */
                error = EVTsetup(ckt);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
/* gtri - end - 6/10/91 - wbk - Setup event-driven data */
                error = DCop(ckt);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
            }
        }
    }
    which = -1;
    for(i=0;i<ANALmaxnum;i++) {
        if(strcmp("TF",analInfo[i]->public.name)==0) {
            which = i;
            break;
        }
    }
    if(which != -1) {
        for(job=task->jobs;job;job=job->JOBnextJob) {
            if(job->JOBtype == which) {
                ckt->CKTcurJob=job;
                error = TFanal(ckt,reset);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
            }
        }
    }
    which = -1;
    for(i=0;i<ANALmaxnum;i++) {
        if(strcmp("DCTransfer",analInfo[i]->public.name)==0) {
            which = i;
            break;
        }
    }
    if(which != -1) {
        for(job=task->jobs;job;job=job->JOBnextJob) {
            if(job->JOBtype == which) {
                ckt->CKTcurJob=job;
                error = CKTic(ckt);
/* gtri - begin - 6/10/91 - wbk - Setup event-driven data */
                error = EVTsetup(ckt);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
/* gtri - end - 6/10/91 - wbk - Setup event-driven data */
                error = DCtrCurv(ckt,reset);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
            }
        }
    }
    which = -1;
    for(i=0;i<ANALmaxnum;i++) {
        if(strcmp("AC",analInfo[i]->public.name)==0) {
            which = i;
            break;
        }
    }
    if(which != -1) {
        for(job=task->jobs;job;job=job->JOBnextJob) {
            if(job->JOBtype == which) {
                ckt->CKTcurJob=job;
                error = CKTic(ckt);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
/* gtri - begin - 6/10/91 - wbk - Setup event-driven data */
                error = EVTsetup(ckt);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
/* gtri - end - 6/10/91 - wbk - Setup event-driven data */
                error = ACan(ckt,reset);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
            }
        }
    }
    which = -1;
    for(i=0;i<ANALmaxnum;i++) {
        if(strcmp("TRAN",analInfo[i]->public.name)==0) {
            which = i;
            break;
        }
    }
    if(which != -1) {
        for(job=task->jobs;job;job=job->JOBnextJob) {
            if(job->JOBtype == which) {
                ckt->CKTcurJob=job;
                ckt->CKTfinalTime = ((TRANan*)job)->TRANfinalTime;
                ckt->CKTstep = ((TRANan*)job)->TRANstep;
                ckt->CKTinitTime = ((TRANan*)job)->TRANinitTime;
                ckt->CKTmaxStep = ((TRANan*)job)->TRANmaxStep;
                if(ckt->CKTmaxStep == 0) {
                    ckt->CKTmaxStep = (ckt->CKTfinalTime-ckt->CKTinitTime)/50;
                }
                ckt->CKTdelmin = 1e-9*ckt->CKTmaxStep;
                ckt->CKTmode = ((TRANan*)job)->TRANmode;
                /* this used to be in setup, but we need it here now */
                /* (must be done after mode is set as above) */
                error = CKTic(ckt);
                if(error) return(error);
/* gtri - begin - 6/10/91 - wbk - Setup event-driven data */
                error = EVTsetup(ckt);
                if(error) {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
/* gtri - end - 6/10/91 - wbk - Setup event-driven data */
                error = DCtran(ckt,reset);
                if(error)  {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
            }
        }
    }
    which = -1;
    for(i=0;i<ANALmaxnum;i++) {
        if(strcmp("PZ",analInfo[i]->public.name)==0) {
            which = i;
            break;
        }
    }
    if(which != -1) {
        for(job=task->jobs;job;job=job->JOBnextJob) {
            if(job->JOBtype == which) {
                ckt->CKTcurJob=job;
                error = CKTic(ckt);
                error = PZan(ckt,reset);
                if(error)  {
                    ckt->CKTstat->STATtotAnalTime += 
                            (*(SPfrontEnd->IFseconds))()-startTime;
                    return(error);
                }
            }
        }
    }
    ckt->CKTstat->STATtotAnalTime += (*(SPfrontEnd->IFseconds))()-startTime;

    /* Temporary */
    if(ckt->CKTsenInfo){
            SENdestroy(ckt->CKTsenInfo);
    }

    return(OK);
}
