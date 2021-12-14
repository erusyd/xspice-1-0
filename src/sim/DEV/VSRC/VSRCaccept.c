/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "VSRCdefs.h"
#include "TRANdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("VSRCaccept.c $Revision: 1.2 $ on $Date: 91/07/11 13:45:20 $")

int
VSRCaccept(ckt,inModel)
    register CKTcircuit *ckt;
    GENmodel *inModel;
        /* set up the breakpoint table.
         */
{
    register VSRCmodel *model = (VSRCmodel *)inModel;
    register VSRCinstance *here;
    int error;

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VSRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VSRCinstances; here != NULL ;
                here=here->VSRCnextInstance) {
            
            if(!ckt->CKTmode & (MODETRAN | MODETRANOP)) {
                /* not transient, so shouldn't be here */
                return(OK);
            } else {
                /* use the transient functions */
                switch(here->VSRCfunctionType) {
                default: { /* no function specified:DC   no breakpoints */
                    break;
                }
                
/* gtri - begin - wbk - add PHASE parameter and simplify algorithm */
                
                case PULSE: {
#define TD ((here->VSRCfunctionOrder >=3)?(*(here->VSRCcoeffs+2)):\
    (0.0))
#define TR ((here->VSRCfunctionOrder >=4)?(*(here->VSRCcoeffs+3)):\
    (ckt->CKTstep))
#define TF ((here->VSRCfunctionOrder >=5)?(*(here->VSRCcoeffs+4)):\
    (ckt->CKTstep))
#define PW ((here->VSRCfunctionOrder >=6)?(*(here->VSRCcoeffs+5)):\
    (ckt->CKTfinalTime))
#define PER ((here->VSRCfunctionOrder>=7)?(*(here->VSRCcoeffs+6)):\
    (ckt->CKTfinalTime))
#define PHASE ((here->VSRCfunctionOrder>=8)?(*(here->VSRCcoeffs+7)):\
    (0.0))

                    double time;
                    double phase;
                    double deltat;
                    double basephase;
                    double basetime;

                    /* offset time by delay and limit to zero */
                    time = ckt->CKTtime - TD;
                    if(time < 0.0)
                        time = 0.0;

                    /* normalize phase to 0 - 2PI */ 
                    phase = PHASE * PI / 180.0;
                    basephase = 2 * PI * floor(phase / (2 * PI));
                    phase -= basephase;

                    /* compute equivalent delta time and add to time */
                    deltat = (phase / (2 * PI)) * PER;
                    time += deltat;

                    /* normalize time to 0 - period */
                    basetime = PER * floor(time/PER);
                    time -= basetime;

                    /* setup the breakpoints */

                    /* if first time and delay != 0, setup a breakpoint */
                    /* to happen right at the delay */
                    if((ckt->CKTtime == 0) && (TD > 0.0)) {
                            error = CKTsetBreak(ckt, TD);
                            if(error) return(error);
                    }

                    /* if time >= delay, setup next breakpoints */
                    if(ckt->CKTtime >= TD) {

                        /* Note that we rely on the CKTsetBreak function */
                        /* to throw out identical breakpoints */

                        /* Note also that the next 2 breakpoints are setup at each */
                        /* time just in case the first one is too close and */
                        /* gets ignored */

                        if(time < TR) {
                            error = CKTsetBreak(ckt, ckt->CKTtime + (TR - time));
                            if(error) return(error);
                            error = CKTsetBreak(ckt, ckt->CKTtime + (TR + PW - time));
                            if(error) return(error);
                        }
                        else if(time < (TR + PW)) {
                            error = CKTsetBreak(ckt, ckt->CKTtime + (TR + PW - time));
                            if(error) return(error);
                            error = CKTsetBreak(ckt, ckt->CKTtime + (TR + PW + TF - time));
                            if(error) return(error);
                        }
                        else if(time < (TR + PW + TF)) {
                            error = CKTsetBreak(ckt, ckt->CKTtime + (TR + PW + TF - time));
                            if(error) return(error);
                            error = CKTsetBreak(ckt, ckt->CKTtime + (PER - time));
                            if(error) return(error);
                        }
                        else /* time < PER */ {
                            error = CKTsetBreak(ckt, ckt->CKTtime + (PER - time));
                            if(error) return(error);
                            error = CKTsetBreak(ckt, ckt->CKTtime + (PER + TR - time));
                            if(error) return(error);
                        }

                    } /* end if CKTtime >= TD */
#undef TD
#undef TR
#undef TF
#undef PW
#undef PER
#undef PHASE
                }
                break;

/* gtri - end - wbk - add PHASE parameter and simplify algorithm */

                case SINE: {
                    /* no  breakpoints (yet) */
                }
                break;
                case EXP: {
                    /* no  breakpoints (yet) */
                }
                break;
                case SFFM:{
                    /* no  breakpoints (yet) */
                }
                break;
                case PWL: {
                    register int i;
                    if(ckt->CKTtime < *(here->VSRCcoeffs)) {
                        if(ckt->CKTbreak) {
                            error = CKTsetBreak(ckt,*(here->VSRCcoeffs));
                            break;
                        }
                    }
                    for(i=0;i<(here->VSRCfunctionOrder/2)-1;i++) {
                        if((*(here->VSRCcoeffs+2*i)==ckt->CKTtime)) {
                            if(ckt->CKTbreak) {
                                error = CKTsetBreak(ckt,
                                        *(here->VSRCcoeffs+2*i+2));
                                if(error) return(error);
                            }
                            goto bkptset;
                        } 
                    }
                    break;
                }
                }
            }
bkptset: ;
        }
    }
    return(OK);
}
