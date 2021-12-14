/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTacLoad(ckt)
     * this is a driver program to iterate through all the various
     * ac load functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "SPerror.h"
/* gtri - add - wbk - 11/26/90 - add include for MIF global data */
#include "MIF.h"
/* gtri - end - wbk - 11/26/90 */
#include "suffix.h"

RCSID("CKTacLoad.c $Revision: 1.3 $ on $Date: 92/08/31 11:33:05 $")

int
CKTacLoad(ckt)
    register CKTcircuit *ckt;
{
    extern SPICEdev *DEVices[];
    register int i;
    register int size;
    int error;

    size = SMPmatSize(ckt->CKTmatrix);
    for (i=0;i<=size;i++) {
        *(ckt->CKTrhs+i)=0;
        *(ckt->CKTirhs+i)=0;
    }
    SMPcClear(ckt->CKTmatrix);

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVacLoad != NULL) && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVacLoad))(ckt->CKThead[i],ckt);
            if(error) return(error);
        }
    }


    /* gtri - begin - Put resistors to ground at all nodes. */
    /* Value of resistor is set by new "rshunt" option.     */

    if(ckt->enh->rshunt_data.enabled) {
       for(i = 0; i < ckt->enh->rshunt_data.num_nodes; i++) {
          *(ckt->enh->rshunt_data.diag[i]) +=
                               ckt->enh->rshunt_data.gshunt;
       }
    }

    /* gtri - end - Put resistors to ground at all nodes */



    /* gtri - add - wbk - 11/26/90 - reset the MIF init flags */

    /* init is set by CKTinit and should be true only for first load call */
    g_mif_info.circuit.init = MIF_FALSE;

    /* anal_init is set by CKTdoJob and is true for first call */
    /* of a particular analysis type */
    g_mif_info.circuit.anal_init = MIF_FALSE;

    /* gtri - end - wbk - 11/26/90 */


    return(OK);
}
