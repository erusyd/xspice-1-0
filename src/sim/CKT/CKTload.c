/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTload(ckt)
     * this is a driver program to iterate through all the various
     * load functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "SPerror.h"
/* gtri - add - wbk - 11/26/90 - add include for MIF global data */
#include "MIF.h"
/* gtri - end - wbk - 11/26/90 */
#include "suffix.h"

RCSID("CKTload.c $Revision: 1.3 $ on $Date: 92/08/28 10:28:27 $")

int
CKTload(ckt)
    register CKTcircuit *ckt;
{
    extern SPICEdev *DEVices[];
    register int i;
    register int size;
    double startTime;
    CKTnode *node;
    int error;
#ifdef STEPDEBUG
    int noncon;
#endif /* STEPDEBUG */

    /* gtri - begin - Put resistors to ground at all nodes */
    SMPmatrix  *matrix;
    double     gshunt;
    int        num_nodes;
    /* gtri - begin - Put resistors to ground at all nodes */

    startTime = (*(SPfrontEnd->IFseconds))();
    size = SMPmatSize(ckt->CKTmatrix);
    for (i=0;i<=size;i++) {
        *(ckt->CKTrhs+i)=0;
    }
    SMPclear(ckt->CKTmatrix);
#ifdef STEPDEBUG
    noncon = ckt->CKTnoncon;
#endif /* STEPDEBUG */

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVload != NULL) && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVload))(ckt->CKThead[i],ckt);
#ifdef STEPDEBUG
            if(noncon != ckt->CKTnoncon) {
                printf("device type %s nonconvergence\n",
                        (*DEVices[i]).DEVpublic.name);
                noncon = ckt->CKTnoncon;
            }
#endif /* STEPDEBUG */
            if(error) return(error);
        }
    }

    /* gtri - add - wbk - 11/26/90 - reset the MIF init flags */

    /* init is set by CKTinit and should be true only for first load call */
    g_mif_info.circuit.init = MIF_FALSE;

    /* anal_init is set by CKTdoJob and is true for first call */
    /* of a particular analysis type */
    g_mif_info.circuit.anal_init = MIF_FALSE;

    /* gtri - end - wbk - 11/26/90 */

    /* gtri - begin - Put resistors to ground at all nodes. */
    /* Value of resistor is set by new "rshunt" option.     */

    if(ckt->enh->rshunt_data.enabled) {
       gshunt = ckt->enh->rshunt_data.gshunt;
       num_nodes = ckt->enh->rshunt_data.num_nodes;
       for(i = 0; i < num_nodes; i++) {
          *(ckt->enh->rshunt_data.diag[i]) += gshunt;
       }
    }

    /* gtri - end - Put resistors to ground at all nodes */

    if(ckt->CKTmode & MODEDC) {
        /* consider doing nodeset & ic assignments */
        if(ckt->CKTmode & (MODEINITJCT | MODEINITFIX)) {
            /* do nodesets */
            for(node=ckt->CKTnodes;node;node=node->next) {
                if(node->nsGiven) {
                    *(ckt->CKTrhs+node->number)+= node->nodeset;
                    *(node->ptr) += 1;
                }
            }
        }
        if( (ckt->CKTmode & MODETRANOP) && (!(ckt->CKTmode & MODEUIC))) {
            for(node=ckt->CKTnodes;node;node=node->next) {
                if(node->icGiven) {
                    *(ckt->CKTrhs+node->number) += node->ic;
                    *(node->ptr) += 1;
                }
            }
        }
    }
    ckt->CKTstat->STATloadTime += (*(SPfrontEnd->IFseconds))()-startTime;
    return(OK);
}
