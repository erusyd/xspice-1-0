/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTsetup(ckt)
     * this is a driver program to iterate through all the various
     * setup functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTsetup.c $Revision: 1.2 $ on $Date: 92/08/28 10:28:48 $")

#define CKALLOC(var,size,type) \
    if(size && (!(var =(type *)MALLOC((size)*sizeof(type))))){\
            return(E_NOMEM);\
}

int
CKTsetup(ckt)
    register CKTcircuit *ckt;

{
    extern SPICEdev *DEVices[];

    register int i;
    int error;

    /* gtri - begin - Setup for adding rshunt option resistors */
    CKTnode *node;
    int     num_nodes;
    /* gtri - end - Setup for adding rshunt option resistors */

    register SMPmatrix *matrix;
    ckt->CKTnumStates=0;


    if(ckt->CKTsenInfo){
        if (error = CKTsenSetup(ckt)) return(error);
    }

    matrix = ckt->CKTmatrix;

    for (i=0;i<DEVmaxnum;i++) {
        if ( ((*DEVices[i]).DEVsetup != NULL) && (ckt->CKThead[i] != NULL) ){
            error = (*((*DEVices[i]).DEVsetup))(matrix,ckt->CKThead[i],ckt,
                    &ckt->CKTnumStates);
            if(error) return(error);
        }
    }
    for(i=0;i<=ckt->CKTmaxOrder+1;i++) {
        CKALLOC(ckt->CKTstates[i],ckt->CKTnumStates,double);
    }
    if(ckt->CKTsenInfo){
        /* to allocate memory to sensitivity structures if 
         * it is not done before */

        error = NIsenReinit(ckt); 
        if(error) return(error);
    }
    if(ckt->CKTniState & NIUNINITIALIZED) {
        error = NIreinit(ckt);
        if(error) return(error);
    }

    /* gtri - begin - Setup for adding rshunt option resistors */

    if(ckt->enh->rshunt_data.enabled) {

        /* Count number of voltage nodes in circuit */
        for(num_nodes = 0, node = ckt->CKTnodes; node; node = node->next)
            if((node->type == NODE_VOLTAGE) && (node->number != 0))
                num_nodes++;
    
        /* Allocate space for the matrix diagonal data */
        if(num_nodes > 0) {
            ckt->enh->rshunt_data.diag =
                 (double **) MALLOC(num_nodes * sizeof(double *));
        }

        /* Set the number of nodes in the rshunt data */
        ckt->enh->rshunt_data.num_nodes = num_nodes;

        /* Get/create matrix diagonal entry following what RESsetup does */
        for(i = 0, node = ckt->CKTnodes; node; node = node->next) {
            if((node->type == NODE_VOLTAGE) && (node->number != 0)) {
                ckt->enh->rshunt_data.diag[i] = 
                      SMPmakeElt(matrix,node->number,node->number);
                i++;
            }
        }

    }
   
    /* gtri - end - Setup for adding rshunt option resistors */
   

    return(OK);
}
