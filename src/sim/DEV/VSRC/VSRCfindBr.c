/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "VSRCdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VSRCfindBr.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:41 $")

int
VSRCfindBr(ckt,inModel,name)
    register CKTcircuit *ckt;
    GENmodel *inModel;
    register IFuid name;
{
    register VSRCmodel *model = (VSRCmodel *)inModel;
    register VSRCinstance *here;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = model->VSRCnextModel) {
        for (here = model->VSRCinstances; here != NULL;
                here = here->VSRCnextInstance) {
            if(here->VSRCname == name) {
                if(here->VSRCbranch == 0) {
                    error = CKTmkCur(ckt,&tmp,here->VSRCname,"branch");
                    if(error) return(error);
                    here->VSRCbranch = tmp->number;
                }
                return(here->VSRCbranch);
            }
        }
    }
    return(0);
}
