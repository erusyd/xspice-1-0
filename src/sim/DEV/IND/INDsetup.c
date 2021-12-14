/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("INDsetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:08 $")

int
INDsetup(matrix,inModel,ckt,states)
    register SMPmatrix *matrix;
    GENmodel *inModel;
    CKTcircuit *ckt;
    int *states;
        /* load the inductor structure with those pointers needed later 
         * for fast matrix loading 
         */
{
    register INDmodel *model = (INDmodel*)inModel;
    register INDinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the inductor models */
    for( ; model != NULL; model = model->INDnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->INDinstances; here != NULL ;
                here=here->INDnextInstance) {
            
            if(here->INDbrEq == 0) {
                error = CKTmkCur(ckt,&tmp,here->INDname,"internal");
                if(error) return(error);
                here->INDbrEq = tmp->number;
            }

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(INDposIbrptr,INDposNode,INDbrEq)
            TSTALLOC(INDnegIbrptr,INDnegNode,INDbrEq)
            TSTALLOC(INDibrNegptr,INDbrEq,INDnegNode)
            TSTALLOC(INDibrPosptr,INDbrEq,INDposNode)
            TSTALLOC(INDibrIbrptr,INDbrEq,INDbrEq)

            here->INDflux = *states;
            *states += 2 ;
            if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN) ){
                *states += 2 * (ckt->CKTsenInfo->SENparms);
            }
        }
    }
    return(OK);
}
