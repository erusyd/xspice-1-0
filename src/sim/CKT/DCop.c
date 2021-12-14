/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "util.h"
#include "SPerror.h"
#include "IFsim.h"

/* gtri - add - wbk - 12/19/90 - Add headers */
#include "MIF.h"
#include "EVT.h"
#include "EVTproto.h"
#include "IPCtiein.h"
/* gtri - end - wbk */

#include "suffix.h"

RCSID("DCop.c $Revision: 1.7 $ on $Date: 92/06/15 18:44:00 $")

int
DCop(ckt) 
    CKTcircuit *ckt;
{
    int i;
    int NIiter();
    int CKTload();
    int converged;
    int error;
    int size;
    long save;
    int senmode;
    IFuid *nameList;
    int numNames;
    GENERIC *plot;
    

/* gtri - add - wbk - 12/19/90 - Add IPC stuff and initialize anal_init and anal_type */

    /* Tell the beginPlot routine what mode we're in */
    g_ipc.anal_type = IPC_ANAL_DCOP;

    /* Tell the code models what mode we're in */
    g_mif_info.circuit.anal_type = MIF_DC;

    g_mif_info.circuit.anal_init = MIF_TRUE;

/* gtri - end - wbk */

    error = CKTnames(ckt,&numNames,&nameList);
    if(error) return(error);
    (*(SPfrontEnd->OUTpBeginPlot))((GENERIC *)ckt,(GENERIC*)ckt->CKTcurJob,
            ckt->CKTcurJob->JOBname,(IFuid)NULL,IF_REAL,numNames,nameList,
            IF_REAL,&plot);

/* gtri - begin - wbk - 6/10/91 - Call EVTop if event-driven instances exist */
    if(ckt->evt->counts.num_insts == 0) {
        /* If no event-driven instances, do what SPICE normally does */
        converged = CKTop(ckt,
            (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITJCT,
            (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITFLOAT,
            ckt->CKTdcMaxIter);
        if(converged != 0) return(converged);
    }
    else {
        /* Else, use new DCOP algorithm */
        converged = EVTop(ckt,
                    (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITJCT,
                    (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITFLOAT,
                    ckt->CKTdcMaxIter,
                    MIF_TRUE);
        EVTdump(ckt, IPC_ANAL_DCOP, 0.0);
        EVTop_save(ckt, MIF_TRUE, 0.0);
        if(converged != 0)
            return(converged);
    }
/* gtri - end - wbk - 6/10/91 - Call EVTop if event-driven instances exist */

    ckt->CKTmode = (ckt->CKTmode & MODEUIC) | MODEDCOP | MODEINITSMSIG;


    if(ckt->CKTsenInfo && ((ckt->CKTsenInfo->SENmode&DCSEN) || 
            (ckt->CKTsenInfo->SENmode&ACSEN)) ){
#ifdef SENSDEBUG
         printf("\nDC Operating Point Sensitivity Results\n\n");
         CKTsenPrint(ckt);
#endif /* SENSDEBUG */
         senmode = ckt->CKTsenInfo->SENmode;
         save = ckt->CKTmode;
         ckt->CKTsenInfo->SENmode = DCSEN;
         size = SMPmatSize(ckt->CKTmatrix);
         for(i = 1; i<=size ; i++){
             *(ckt->CKTrhsOp + i) = *(ckt->CKTrhsOld + i);
         }
         if(error = CKTsenDCtran(ckt)) return(error);
         ckt->CKTmode = save;
         ckt->CKTsenInfo->SENmode = senmode;

    }
    converged = CKTload(ckt);

/* gtri - modify - wbk - 12/19/90 - Send IPC data delimiters */

    if(g_ipc.enabled)
        ipc_send_dcop_prefix();

    CKTdump(ckt,(double)0,plot);

    if(g_ipc.enabled)
        ipc_send_dcop_suffix();

/* gtri - end - wbk */

    (*(SPfrontEnd->OUTendPlot))(plot);
    return(converged);
}
