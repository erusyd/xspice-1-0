/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* load the voltage source structure with those pointers needed later 
     * for fast matrix loading 
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "VCVSdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSsetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:31 $")

/*ARGSUSED*/
int
VCVSsetup(matrix,inModel,ckt,states)
    register SMPmatrix *matrix;
    GENmodel *inModel;
    register CKTcircuit *ckt;
    int *states;
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VCVSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VCVSinstances; here != NULL ;
                here=here->VCVSnextInstance) {
            
            if(here->VCVSbranch == 0) {
                error = CKTmkCur(ckt,&tmp,here->VCVSname,"branch");
                if(error) return(error);
                here->VCVSbranch = tmp->number;
            }

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(VCVSposIbrptr, VCVSposNode, VCVSbranch)
            TSTALLOC(VCVSnegIbrptr, VCVSnegNode, VCVSbranch)
            TSTALLOC(VCVSibrNegptr, VCVSbranch, VCVSnegNode)
            TSTALLOC(VCVSibrPosptr, VCVSbranch, VCVSposNode)
            TSTALLOC(VCVSibrContPosptr, VCVSbranch, VCVScontPosNode)
            TSTALLOC(VCVSibrContNegptr, VCVSbranch, VCVScontNegNode)
        }
    }
    return(OK);
}
