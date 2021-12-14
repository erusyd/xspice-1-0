/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "ISRCdefs.h"
#include "util.h"
#include <math.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("ISRCtemp.c $Revision: 1.1 $ on $Date: 91/07/11 11:06:38 $")

/*ARGSUSED*/
int
ISRCtemp(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    register ISRCmodel *model = (ISRCmodel*)inModel;
    register ISRCinstance *here;
    double radians;

    for( ; model != NULL; model = model->ISRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->ISRCinstances; here != NULL ;
                here=here->ISRCnextInstance) {

            if(here->ISRCacGiven && !here->ISRCacMGiven) {
                here->ISRCacMag=1;
            }
            if(here->ISRCacGiven && !here->ISRCacPGiven) {
                here->ISRCacPhase=0;
            }
            if(!here->ISRCdcGiven) {
                /* no DC value - either have a transient value, or none */
                if(here->ISRCfuncTGiven) {
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
            "Source %s has no DC value, transient time 0 value used",
                            &(here->ISRCname));
                } else {
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "Source %s has no value, DC 0 assumed\n",
                            &(here->ISRCname));
                }
            }
            radians = here->ISRCacPhase * PI / 180;
            here->ISRCacReal = here->ISRCacMag * cos(radians);
            here->ISRCacImag = here->ISRCacMag * sin(radians);
        }
    }
    return(OK);

}
