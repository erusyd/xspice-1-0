/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

        /* load the inductor structure with those pointers needed later 
         * for fast matrix loading 
         */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "IFsim.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "INDdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("MUTsetup.c $Revision: 1.1 $ on $Date: 90/10/11 12:43:19 $")

#ifdef MUTUAL
/*ARGSUSED*/
int
MUTsetup(matrix,inModel,ckt,states)
    register SMPmatrix *matrix;
    GENmodel *inModel;
    CKTcircuit *ckt;
    int *states;
{
    register MUTmodel *model = (MUTmodel*)inModel;
    register MUTinstance *here;
    int ktype;
    int error;

    /*  loop through all the inductor models */
    for( ; model != NULL; model = model->MUTnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->MUTinstances; here != NULL ;
                here=here->MUTnextInstance) {
            
            ktype = CKTtypelook("Inductor");
            if(ktype <= 0) {
                (*(SPfrontEnd->IFerror))(ERR_PANIC,
                        "mutual inductor, but inductors not available!",
                        (IFuid *)NULL);
                return(E_INTERN);
            }

            error = CKTfndDev((GENERIC*)ckt,&ktype,(GENERIC**)&(here->MUTind1),
                    here->MUTindName1, (GENERIC *)NULL,(char *)NULL);
            if(error && error!= E_NODEV && error != E_NOMOD) return(error);
            if(error) {
                IFuid namarray[2];
                namarray[0]=here->MUTname;
                namarray[1]=here->MUTindName1;
                (*(SPfrontEnd->IFerror))(ERR_WARNING,
                    "%s: coupling to non-existant inductor %s.",
                    namarray);
            }
            error = CKTfndDev((GENERIC*)ckt,&ktype,(GENERIC**)&(here->MUTind2),
                    here->MUTindName2,(GENERIC *)NULL,(char *)NULL);
            if(error && error!= E_NODEV && error != E_NOMOD) return(error);
            if(error) {
                IFuid namarray[2];
                namarray[0]=here->MUTname;
                namarray[1]=here->MUTindName2;
                (*(SPfrontEnd->IFerror))(ERR_WARNING,
                    "%s: coupling to non-existant inductor %s.",
                    namarray);
            }

            here->MUTfactor = here->MUTcoupling *sqrt(here->MUTind1->INDinduct *
                    here->MUTind2->INDinduct);


/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

            TSTALLOC(MUTbr1br2,MUTind1->INDbrEq,MUTind2->INDbrEq)
            TSTALLOC(MUTbr2br1,MUTind2->INDbrEq,MUTind1->INDbrEq)
        }
    }
    return(OK);
}
#endif /* MUTUAL */
