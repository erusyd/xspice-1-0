/*
 * Copyright (c) 1987 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "ASRCdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("ASRCfindBranch.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:27 $")

int
ASRCfindBr(ckt,inputModel,name)
    register CKTcircuit *ckt;
    GENmodel *inputModel;
    register IFuid name;
{
    register ASRCinstance *here;
    register ASRCmodel *model = (ASRCmodel*)inputModel;
    int error;
    CKTnode *tmp;

    for( ; model != NULL; model = model->ASRCnextModel) {
        for (here = model->ASRCinstances; here != NULL;
                here = here->ASRCnextInstance) {
            if(here->ASRCname == name) {
                if(here->ASRCbranch == 0) {
                    error = CKTmkCur(ckt,&tmp, here->ASRCname,"current");
                    if(error) return(error);
                    here->ASRCbranch = tmp->number;
                }
                return(here->ASRCbranch);
            }
        }
    }
    return(0);
}
