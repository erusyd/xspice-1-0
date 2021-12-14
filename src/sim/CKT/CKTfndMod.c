/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "IFsim.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTfndMod.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:32 $")

int
CKTfndMod(ckt,type,modfast,modname)
    GENERIC *ckt;
    int *type;
    GENERIC **modfast;
    IFuid modname;
{
    register GENmodel *mods;

    if(modfast != NULL && *(GENmodel **)modfast != NULL) {
        /* already have  modfast, so nothing to do */
        if(type) *type = (*(GENmodel **)modfast)->GENmodType;
        return(OK);
    } 
    if(*type >=0 && *type < DEVmaxnum) {
        /* have device type, need to find model */
        /* look through all models */
        for(mods=((CKTcircuit *)ckt)->CKThead[*type]; mods != NULL ; 
                mods = mods->GENnextModel) {
            if(mods->GENmodName == modname) {
                *modfast = (char *)mods;
                return(OK);
            }
        }
        return(E_NOMOD);
    } else if(*type == -1) {
        /* look through all types (UGH - worst case - take forever) */ 
        for(*type = 0;*type <DEVmaxnum;(*type)++) {
            /* need to find model & device */
            /* look through all models */
            for(mods=((CKTcircuit *)ckt)->CKThead[*type];mods!=NULL;
                    mods = mods->GENnextModel) {
                if(mods->GENmodName == modname) {
                    *modfast = (char *)mods;
                    return(OK);
                }
            }
        }
        *type = -1;
        return(E_NOMOD);
    } else return(E_BADPARM);
}
