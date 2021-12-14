/*
 * Copyright (c) 1985 Gordon Jacobs
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "SWdefs.h"
#include "util.h"
#include "TRANdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SWload.c $Revision: 1.1 $ on $Date: 90/10/11 12:46:21 $")

int
SWload(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
        /* actually load the current values into the 
         * sparse matrix previously provided 
         */
{
    register SWmodel *model = (SWmodel *) inModel;
    register SWinstance *here;
    double g_now;
    double v_ctrl;
    double previous_state; 
    double current_state;

    /*  loop through all the switch models */
    for( ; model != NULL; model = model->SWnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->SWinstances; here != NULL ;
                here=here->SWnextInstance) {

            /* decide the state of the switch */

            if(ckt->CKTmode & (MODEINITFIX|MODEINITJCT)) {

                if(here->SWzero_stateGiven) {
                        /* switch specified "on" */
                    *(ckt->CKTstate0 + here->SWstate) = 1.0;
                    current_state = 1.0;
                } else {
                    *(ckt->CKTstate0 + here->SWstate) = 0.0;
                    current_state = 0.0;
                }

            } else if (ckt->CKTmode & (MODEINITSMSIG)) {

                previous_state = *(ckt->CKTstate0 + here->SWstate);
                current_state = previous_state;

            } else if (ckt->CKTmode & (MODEINITFLOAT)) {

                /* use state0 since INITTRAN or INITPRED already called */
                previous_state = *(ckt->CKTstate0 + here->SWstate);
                v_ctrl = *(ckt->CKTrhsOld + here->SWposCntrlNode)
                         - *(ckt->CKTrhsOld + here->SWnegCntrlNode);
                if(v_ctrl > (model->SWvThreshold + model->SWvHysteresis)) {
                    *(ckt->CKTstate0 + here->SWstate) = 1.0;
                    current_state = 1.0;
                } else if(v_ctrl < (model->SWvThreshold - 
                        model->SWvHysteresis)) {
                    *(ckt->CKTstate0 + here->SWstate) = 0.0;
                    current_state = 0.0;
                } else {
                    current_state = previous_state;
                }

                if(current_state != previous_state) {
                    ckt->CKTnoncon++;       /* ensure one more iteration */
                }


            } else if(ckt->CKTmode & (MODEINITTRAN|MODEINITPRED) ) {

                previous_state = *(ckt->CKTstate1 + here->SWstate);
                v_ctrl = *(ckt->CKTrhsOld + here->SWposCntrlNode)
                         - *(ckt->CKTrhsOld + here->SWnegCntrlNode);

                if(v_ctrl > (model->SWvThreshold + model->SWvHysteresis))  {
                    current_state = 1.0;
                } else if(v_ctrl < (model->SWvThreshold - 
                        model->SWvHysteresis)) {
                    current_state = 0.0;
                } else {
                    current_state = previous_state;
                }

                if(current_state == 0) {
                    *(ckt->CKTstate0 + here->SWstate) = 0.0;
                } else {
                    *(ckt->CKTstate0 + here->SWstate) = 1.0;
                }

            }

            g_now = current_state?(model->SWonConduct):(model->SWoffConduct);
            here->SWcond = g_now;

            *(here->SWposPosptr) += g_now;
            *(here->SWposNegptr) -= g_now;
            *(here->SWnegPosptr) -= g_now;
            *(here->SWnegNegptr) += g_now;
        }
    }
    return(OK);
}
