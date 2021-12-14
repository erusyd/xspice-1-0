/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "JFETdefs.h"
#include "CONST.h"
#include <math.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("JFETsetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:51 $")

int
JFETsetup(matrix,inModel,ckt,states)
    register SMPmatrix *matrix;
    GENmodel *inModel;
    CKTcircuit *ckt;
    int *states;
        /* load the diode structure with those pointers needed later 
         * for fast matrix loading 
         */
{
    register JFETmodel *model = (JFETmodel*)inModel;
    register JFETinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the diode models */
    for( ; model != NULL; model = model->JFETnextModel ) {

        if( (model->JFETtype != NJF) && (model->JFETtype != PJF) ) {
            model->JFETtype = NJF;
        }
        if(!model->JFETthresholdGiven) {
            model->JFETthreshold = -2;
        }
        if(!model->JFETbetaGiven) {
            model->JFETbeta = 1e-4;
        }
        if(!model->JFETlModulationGiven) {
            model->JFETlModulation = 0;
        }
        if(!model->JFETdrainResistGiven) {
            model->JFETdrainResist = 0;
        }
        if(!model->JFETsourceResistGiven) {
            model->JFETsourceResist = 0;
        }
        if(!model->JFETcapGSGiven) {
            model->JFETcapGS = 0;
        }
        if(!model->JFETcapGDGiven) {
            model->JFETcapGD = 0;
        }
        if(!model->JFETgatePotentialGiven) {
            model->JFETgatePotential = 1;
        }
        if(!model->JFETgateSatCurrentGiven) {
            model->JFETgateSatCurrent = 1e-14;
        }
        if(!model->JFETdepletionCapCoeffGiven) {
            model->JFETdepletionCapCoeff = .5;
        }

        if(model->JFETdrainResist != 0) {
            model->JFETdrainConduct = 1/model->JFETdrainResist;
        } else {
            model->JFETdrainConduct = 0;
        }
        if(model->JFETsourceResist != 0) {
            model->JFETsourceConduct = 1/model->JFETsourceResist;
        } else {
            model->JFETsourceConduct = 0;
        }

        /* loop through all the instances of the model */
        for (here = model->JFETinstances; here != NULL ;
                here=here->JFETnextInstance) {
            
            if(!here->JFETareaGiven) {
                here->JFETarea = 1;
            }
            here->JFETstate = *states;
            *states += 13;

            if(model->JFETsourceResist != 0 && here->JFETsourcePrimeNode==0) {
                error = CKTmkVolt(ckt,&tmp,here->JFETname,"source");
                if(error) return(error);
                here->JFETsourcePrimeNode = tmp->number;
            } else {
                here->JFETsourcePrimeNode = here->JFETsourceNode;
            }
            if(model->JFETdrainResist != 0 && here->JFETdrainPrimeNode==0) {
                error = CKTmkVolt(ckt,&tmp,here->JFETname,"drain");
                if(error) return(error);
                here->JFETdrainPrimeNode = tmp->number;
            } else {
                here->JFETdrainPrimeNode = here->JFETdrainNode;
            }

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(JFETdrainDrainPrimePtr,JFETdrainNode,JFETdrainPrimeNode)
            TSTALLOC(JFETgateDrainPrimePtr,JFETgateNode,JFETdrainPrimeNode)
            TSTALLOC(JFETgateSourcePrimePtr,JFETgateNode,JFETsourcePrimeNode)
            TSTALLOC(JFETsourceSourcePrimePtr,JFETsourceNode,
                    JFETsourcePrimeNode)
            TSTALLOC(JFETdrainPrimeDrainPtr,JFETdrainPrimeNode,JFETdrainNode)
            TSTALLOC(JFETdrainPrimeGatePtr,JFETdrainPrimeNode,JFETgateNode)
            TSTALLOC(JFETdrainPrimeSourcePrimePtr,JFETdrainPrimeNode,
                    JFETsourcePrimeNode)
            TSTALLOC(JFETsourcePrimeGatePtr,JFETsourcePrimeNode,JFETgateNode)
            TSTALLOC(JFETsourcePrimeSourcePtr,JFETsourcePrimeNode,
                    JFETsourceNode)
            TSTALLOC(JFETsourcePrimeDrainPrimePtr,JFETsourcePrimeNode,
                    JFETdrainPrimeNode)
            TSTALLOC(JFETdrainDrainPtr,JFETdrainNode,JFETdrainNode)
            TSTALLOC(JFETgateGatePtr,JFETgateNode,JFETgateNode)
            TSTALLOC(JFETsourceSourcePtr,JFETsourceNode,JFETsourceNode)
            TSTALLOC(JFETdrainPrimeDrainPrimePtr,JFETdrainPrimeNode,
                    JFETdrainPrimeNode)
            TSTALLOC(JFETsourcePrimeSourcePrimePtr,JFETsourcePrimeNode,
                    JFETsourcePrimeNode)
        }
    }
    return(OK);
}
