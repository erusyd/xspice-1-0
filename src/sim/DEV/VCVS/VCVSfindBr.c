/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "CKTdefs.h"
#include "VCVSdefs.h"
#include "TRANdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VCVSfindBr.c $Revision: 1.1 $ on $Date: 90/10/11 12:47:25 $")

int
VCVSfindBr(ckt,inModel,name)
    register CKTcircuit *ckt;
    GENmodel *inModel;
    register IFuid name;
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = model->VCVSnextModel) {
        for (here = model->VCVSinstances; here != NULL;
                here = here->VCVSnextInstance) {
            if(here->VCVSname == name) {
                if(here->VCVSbranch == 0) {
                    error = CKTmkCur(ckt,&tmp,here->VCVSname,"branch");
                    if(error) return(error);
                    here->VCVSbranch = tmp->number;
                }
                return(here->VCVSbranch);
            }
        }
    }
    return(0);
}
