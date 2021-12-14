/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "VSRCdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VSRCtemp.c $Revision: 1.1 $ on $Date: 91/07/11 11:05:59 $")

/* ARGSUSED */
int
VSRCtemp(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /* Pre-process voltage source parameters 
         */
{
    register VSRCmodel *model = (VSRCmodel *)inModel;
    register VSRCinstance *here;
    double radians;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VSRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VSRCinstances; here != NULL ;
                here=here->VSRCnextInstance) {
            
            if(here->VSRCacGiven && !here->VSRCacMGiven) {
                here->VSRCacMag = 1;
            }
            if(here->VSRCacGiven && !here->VSRCacPGiven) {
                here->VSRCacPhase = 0;
            }
            if(!here->VSRCdcGiven) {
                /* no DC value - either have a transient value, or none */
                if(here->VSRCfuncTGiven) {
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "%s: no DC value, transient time 0 value used",
                            &(here->VSRCname));
                } else {
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "%s: has no value, DC 0 assumed",
                            &(here->VSRCname));
                }
            }
            radians = here->VSRCacPhase * PI / 180;
            here->VSRCacReal = here->VSRCacMag * cos(radians);
            here->VSRCacImag = here->VSRCacMag * sin(radians);

        }
    }
    return(OK);
}
