/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* load the diode structure with those pointers needed later 
     * for fast matrix loading 
     */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("DIOsetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:47 $")

int
DIOsetup(matrix,inModel,ckt,states)
    register SMPmatrix *matrix;
    GENmodel *inModel;
    CKTcircuit *ckt;
    int *states;
{
    register DIOmodel *model = (DIOmodel*)inModel;
    register DIOinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the diode models */
    for( ; model != NULL; model = model->DIOnextModel ) {

        if(!model->DIOemissionCoeffGiven) {
            model->DIOemissionCoeff = 1;
        }
        if(!model->DIOsatCurGiven) {
            model->DIOsatCur = 1e-14;
        }
        if(!model->DIObreakdownCurrentGiven) {
            model->DIObreakdownCurrent = 1e-3;
        }
        if(!model->DIOjunctionPotGiven){
            model->DIOjunctionPot = 1;
        }
        if(!model->DIOgradingCoeffGiven) {
            model->DIOgradingCoeff = .5;
        } 
        if(!model->DIOdepletionCapCoeffGiven) {
            model->DIOdepletionCapCoeff = .5;
        } 
        if(!model->DIOtransitTimeGiven) {
            model->DIOtransitTime = 0;
        }
        if(!model->DIOjunctionCapGiven) {
            model->DIOjunctionCap = 0;
        }
        if(!model->DIOactivationEnergyGiven) {
            model->DIOactivationEnergy = 1.11;
        } 
        if(!model->DIOsaturationCurrentExpGiven) {
            model->DIOsaturationCurrentExp = 3;
        }

        /* loop through all the instances of the model */
        for (here = model->DIOinstances; here != NULL ;
                here=here->DIOnextInstance) {
            
            if(!here->DIOareaGiven) {
                here->DIOarea = 1;
            }
            if(model->DIOresist == 0) {
                here->DIOposPrimeNode = here->DIOposNode;
            } else if(here->DIOposPrimeNode == 0) {
                error = CKTmkVolt(ckt,&tmp,here->DIOname,"internal");
                if(error) return(error);
                here->DIOposPrimeNode = tmp->number;
            }

            here->DIOstate = *states;
            *states += 5;
            if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN) ){
                *states += 2 * (ckt->CKTsenInfo->SENparms);
            }

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(DIOposPosPrimePtr,DIOposNode,DIOposPrimeNode)
            TSTALLOC(DIOnegPosPrimePtr,DIOnegNode,DIOposPrimeNode)
            TSTALLOC(DIOposPrimePosPtr,DIOposPrimeNode,DIOposNode)
            TSTALLOC(DIOposPrimeNegPtr,DIOposPrimeNode,DIOnegNode)
            TSTALLOC(DIOposPosPtr,DIOposNode,DIOposNode)
            TSTALLOC(DIOnegNegPtr,DIOnegNode,DIOnegNode)
            TSTALLOC(DIOposPrimePosPrimePtr,DIOposPrimeNode,DIOposPrimeNode)
        }
    }
    return(OK);
}
