/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "ACdefs.h"
#include "util.h"
#include "SPerror.h"

/* gtri - add - wbk - 12/19/90 - Add headers */
#include "MIF.h"
#include "EVTproto.h"
#include "IPCtiein.h"
/* gtri - end - wbk */

#include "suffix.h"

RCSID("ACan.c $Revision: 1.6 $ on $Date: 92/06/15 18:43:40 $")

int
ACan(ckt,restart) 
CKTcircuit *ckt;
int restart;
{

    double freq;
    double freqTol; /* tolerence parameter for finding final frequency */
    int error;
    long save;
    int save1;
    int numNames;
    IFuid *nameList;
    IFuid freqUid;
    GENERIC *acPlot;

/* gtri - add - wbk - 12/19/90 - Add IPC stuff and anal_init and anal_type */

    /* Tell the beginPlot routine what mode we're in */
    g_ipc.anal_type = IPC_ANAL_AC;

    /* Tell the code models what mode we're in */
    g_mif_info.circuit.anal_type = MIF_DC;

    g_mif_info.circuit.anal_init = MIF_TRUE;

/* gtri - end - wbk */

    if(((ACAN*)ckt->CKTcurJob)->ACsaveFreq == 0 || restart) { 
        /* start at beginning */

        switch(((ACAN*)ckt->CKTcurJob)->ACstepType) {

        case DECADE:
            ((ACAN*)ckt->CKTcurJob)->ACfreqDelta =
                    exp(log(10.0)/((ACAN*)ckt->CKTcurJob)->ACnumberSteps);
            break;
        case OCTAVE:
            ((ACAN*)ckt->CKTcurJob)->ACfreqDelta =
                    exp(log(2.0)/((ACAN*)ckt->CKTcurJob)->ACnumberSteps);
            break;
        case LINEAR:
            ((ACAN*)ckt->CKTcurJob)->ACfreqDelta =
                    (((ACAN*)ckt->CKTcurJob)->ACstopFreq -
                    ((ACAN*)ckt->CKTcurJob)->ACstartFreq)/
                    (((ACAN*)ckt->CKTcurJob)->ACnumberSteps+1);
            break;
        default:
            return(E_BADPARM);
        }
/* gtri - begin - wbk - Call EVTop if event-driven instances exist */
        if(ckt->evt->counts.num_insts == 0) {
            /* If no event-driven instances, do what SPICE normally does */
            error = CKTop(ckt,
                (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITJCT,
                (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITFLOAT,
                ckt->CKTdcMaxIter);
            if(error) return(error);
        }
        else {
            /* Else, use new DCOP algorithm */
            error = EVTop(ckt,
                        (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITJCT,
                        (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITFLOAT,
                        ckt->CKTdcMaxIter,
                        MIF_TRUE);
            EVTdump(ckt, IPC_ANAL_DCOP, 0.0);
            EVTop_save(ckt, MIF_TRUE, 0.0);
            if(error)
                return(error);
        }
/* gtri - end - wbk - Call EVTop if event-driven instances exist */

/* gtri - add - wbk - 12/19/90 - Add IPC stuff */

        /* Send the operating point results for Mspice compatibility */
        if(g_ipc.enabled) {

            /* Call CKTnames to get names of nodes/branches used by BeginPlot */
            /* Probably should free nameList after this block since called again... */
            error = CKTnames(ckt,&numNames,&nameList);
            if(error) return(error);

            /* We have to do a beginPlot here since the data to return is */
            /* different for the DCOP than it is for the AC analysis.  Moreover */
            /* the begin plot has not even been done yet at this point... */
            (*(SPfrontEnd->OUTpBeginPlot))((GENERIC *)ckt,(GENERIC*)ckt->CKTcurJob,
                ckt->CKTcurJob->JOBname,(IFuid)NULL,IF_REAL,numNames,nameList,
                IF_REAL,&acPlot);

            ipc_send_dcop_prefix();
            CKTdump(ckt,(double)0,acPlot);
            ipc_send_dcop_suffix();

            (*(SPfrontEnd->OUTendPlot))(acPlot);
        }

/* gtri - end - wbk */

        ckt->CKTmode = (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITSMSIG;
        error = CKTload(ckt);
        if(error) return(error);

        error = CKTnames(ckt,&numNames,&nameList);
        if(error) return(error);
        (*(SPfrontEnd->IFnewUid))((GENERIC *)ckt,&freqUid,(IFuid)NULL,
                "frequency", UID_OTHER,(GENERIC **)NULL);
        (*(SPfrontEnd->OUTpBeginPlot))((GENERIC *)ckt,(GENERIC*)ckt->CKTcurJob,
                ckt->CKTcurJob->JOBname,freqUid,IF_REAL,numNames,nameList,
                IF_COMPLEX,&acPlot);
        freq = ((ACAN*)ckt->CKTcurJob)->ACstartFreq;

    } else {    /* continue previous analysis */
        freq = ((ACAN*)ckt->CKTcurJob)->ACsaveFreq;
        ((ACAN*)ckt->CKTcurJob)->ACsaveFreq = 0; /* clear the 'old' frequency */
    }
    switch(((ACAN*)ckt->CKTcurJob)->ACstepType) {
    case DECADE:
    case OCTAVE:
        freqTol = ((ACAN*)ckt->CKTcurJob)->ACfreqDelta * 
                ((ACAN*)ckt->CKTcurJob)->ACstopFreq * ckt->CKTreltol;
        break;
    case LINEAR:
        freqTol = ((ACAN*)ckt->CKTcurJob)->ACfreqDelta * ckt->CKTreltol;
        break;
    default:
        return(E_BADPARM);
    }

/* gtri - add - wbk - 12/19/90 - Set anal_init and anal_type */

    g_mif_info.circuit.anal_init = MIF_TRUE;

    /* Tell the code models what mode we're in */
    g_mif_info.circuit.anal_type = MIF_AC;

/* gtri - end - wbk */

    while(freq <= ((ACAN*)ckt->CKTcurJob)->ACstopFreq+freqTol) {

        if( (*(SPfrontEnd->IFpauseTest))() ) { 
            /* user asked us to pause via an interrupt */
            ((ACAN*)ckt->CKTcurJob)->ACsaveFreq = freq;
            return(E_PAUSE);
        }
        ckt->CKTomega = 2 * PI *freq;
        ckt->CKTmode = (ckt->CKTmode&MODEUIC) | MODEAC;

        error = NIacIter(ckt);
        if(error) return(error);


        if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode&ACSEN) ){

            save = ckt->CKTmode;
            ckt->CKTmode=(ckt->CKTmode&MODEUIC)|MODEDCOP|MODEINITSMSIG;
            save1 = ckt->CKTsenInfo->SENmode;
            ckt->CKTsenInfo->SENmode = ACSEN;
            if(freq == ((ACAN*)ckt->CKTcurJob)->ACstartFreq){
                ckt->CKTsenInfo->SENacpertflag = 1;
            }
            else{
                ckt->CKTsenInfo->SENacpertflag = 0;
            }
            if(error = CKTsenAC(ckt)) return (error);
            ckt->CKTmode = save;
            ckt->CKTsenInfo->SENmode = save1;
        }

/* gtri - modify - wbk - 12/19/90 - Send IPC stuff */

        if(g_ipc.enabled)
            ipc_send_data_prefix(freq);

        error = CKTacDump(ckt,freq,acPlot);

        if(g_ipc.enabled)
            ipc_send_data_suffix();

        if(error) return(error);

/* gtri - modify - wbk - 12/19/90 - Send IPC stuff */

        /*  increment frequency */

        switch(((ACAN*)ckt->CKTcurJob)->ACstepType) {
        case DECADE:
        case OCTAVE:
            freq *= ((ACAN*)ckt->CKTcurJob)->ACfreqDelta;
            if(((ACAN*)ckt->CKTcurJob)->ACfreqDelta==1) goto endsweep;
            break;
        case LINEAR:
            freq += ((ACAN*)ckt->CKTcurJob)->ACfreqDelta;
            if(((ACAN*)ckt->CKTcurJob)->ACfreqDelta==0) goto endsweep;
            break;
        default:
            return(E_INTERN);
        }
    }
endsweep:
    (*(SPfrontEnd->OUTendPlot))(acPlot);
    return(0);
}
