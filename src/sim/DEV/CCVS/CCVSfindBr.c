/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "CCVSdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("CCVSfindBranch.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:04 $")

int
CCVSfindBr(ckt,inModel,name)
    register CKTcircuit *ckt;
    GENmodel *inModel;
    register IFuid name;
{
    register CCVSmodel *model = (CCVSmodel*)inModel;
    register CCVSinstance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = model->CCVSnextModel) {
        for (here = model->CCVSinstances; here != NULL;
                here = here->CCVSnextInstance) {
            if(here->CCVSname == name) {
                if(here->CCVSbranch == 0) {
                    error = CKTmkCur(ckt,&tmp, here->CCVSname,"current");
                    if(error) return(error);
                    here->CCVSbranch = tmp->number;
                }
                return(here->CCVSbranch);
            }
        }
    }
    return(0);
}
