/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CSWdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CSWsetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:25 $")

int
CSWsetup(matrix,inModel,ckt,states)
    register SMPmatrix *matrix;
    GENmodel *inModel;
    CKTcircuit *ckt;
    int *states;
        /* load the switch conductance with those pointers needed later 
         * for fast matrix loading 
         */

{
    register CSWmodel *model = (CSWmodel*)inModel;
    register CSWinstance *here;

    /*  loop through all the current source models */
    for( ; model != NULL; model = model->CSWnextModel ) {
        /* Default Value Processing for Switch Model */
        if (!model->CSWthreshGiven) {
            model->CSWiThreshold = 0;
        }
        if (!model->CSWhystGiven) {
            model->CSWiHysteresis = 0;
        }
        if (!model->CSWonGiven)  {
            model->CSWonConduct = CSW_ON_CONDUCTANCE;
            model->CSWonResistance = 1.0/model->CSWonConduct;
        }
        if (!model->CSWoffGiven)  {
            model->CSWoffConduct = CSW_OFF_CONDUCTANCE;
            model->CSWoffResistance = 1.0/model->CSWoffConduct;
        }

        /* loop through all the instances of the model */
        for (here = model->CSWinstances; here != NULL ;
                here=here->CSWnextInstance) {

            /* Default Value Processing for Switch Instance */

            here->CSWcontBranch = CKTfndBranch(ckt,here->CSWcontName);
            if(here->CSWcontBranch == NULL) {
                IFuid namarray[2];
                namarray[0] = here->CSWname;
                namarray[1] = here->CSWcontName;
                (*(SPfrontEnd->IFerror))(ERR_FATAL,
                        "%s: unknown controlling source %s",namarray);
                return(E_BADPARM);
            }

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(CSWposPosptr, CSWposNode, CSWposNode)
            TSTALLOC(CSWposNegptr, CSWposNode, CSWnegNode)
            TSTALLOC(CSWnegPosptr, CSWnegNode, CSWposNode)
            TSTALLOC(CSWnegNegptr, CSWnegNode, CSWnegNode)
            here->CSWstate = *states;
            *states += CSW_NUM_STATES;
        }
    }
    return(OK);
}
