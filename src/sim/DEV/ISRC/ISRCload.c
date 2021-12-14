/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "ISRCdefs.h"
#include "util.h"
#include "TRANdefs.h"
#include <math.h>
#include "SPerror.h"

/* gtri - begin - wbk - modify for supply ramping option */
#include "CMproto.h"
/* gtri - end   - wbk - modify for supply ramping option */

#include "suffix.h"

RCSID("ISRCload.c $Revision: 1.5 $ on $Date: 92/01/05 12:49:10 $")

int
ISRCload(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
        /* actually load the current current value into the 
         * sparse matrix previously provided 
         */
{
    register ISRCmodel *model = (ISRCmodel*)inModel;
    register ISRCinstance *here;
    double value;
    double time;

    /*  loop through all the current source models */
    for( ; model != NULL; model = model->ISRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->ISRCinstances; here != NULL ;
                here=here->ISRCnextInstance) {
            

/* gtri - begin - wbk - modify to process srcFact, etc. for all sources */

            if( (ckt->CKTmode & (MODEDCOP | MODEDCTRANCURVE)) &&
                    here->ISRCdcGiven ) {
                /* load using DC value */
                value = here->ISRCdcValue;
            } else {
                if(ckt->CKTmode & (MODEDC)) {
                    time = 0;
                } else {
                    time = ckt->CKTtime;
                }
                /* use transient function */
                switch(here->ISRCfunctionType) {

/* gtri - begin - wbk - add PHASE parameter */

                case PULSE: {
#define V1 (*(here->ISRCcoeffs))
#define V2 (*(here->ISRCcoeffs+1))
#define TD ((here->ISRCfunctionOrder >=3)?(*(here->ISRCcoeffs+2)):\
    (0.0))
#define TR (((here->ISRCfunctionOrder >=4) && (*(here->ISRCcoeffs+3)))? \
    (*(here->ISRCcoeffs+3)):(ckt->CKTstep))
#define TF (((here->ISRCfunctionOrder >=5) && (*(here->ISRCcoeffs+4)))? \
    (*(here->ISRCcoeffs+4)):(ckt->CKTstep))
#define PW (((here->ISRCfunctionOrder >=6) && (*(here->ISRCcoeffs+5)))? \
    (*(here->ISRCcoeffs+5)):(ckt->CKTfinalTime))
#define PER (((here->ISRCfunctionOrder>=7) && (*(here->ISRCcoeffs+6)))? \
    (*(here->ISRCcoeffs+6)):(ckt->CKTfinalTime))
#define PHASE (((here->ISRCfunctionOrder>=8) && (*(here->ISRCcoeffs+7)))? \
    (*(here->ISRCcoeffs+7)):(0.0))
                    double phase;
                    double deltat;
                    double basephase;
                    double basetime;

                    /* offset time by delay and limit to zero */
                    time -= TD;
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

                    /* compute the waveform value for the given time */
                    if( time <= 0 || time >= TR + PW + TF) {
                        value = V1;
                    } else  if ( time >= TR && time <= TR + PW) {
                        value = V2;
                    } else if (time > 0 && time < TR) {
                        value = V1 + (V2 - V1) * (time) / TR;
                    } else { /* time > TR + PW && < TR + PW + TF */
                        value = V2 + (V1 - V2) * (time - (TR + PW)) / TF;
                    }
#undef V1
#undef V2
#undef TD
#undef TR
#undef TF
#undef PW
#undef PER
#undef PHASE
                }
                break;

/* gtri - end - wbk - add PHASE parameter */

/* gtri - begin - wbk - add PHASE parameter */

                case SINE: {
#define VO    (*(here->ISRCcoeffs))
#define VA    (*(here->ISRCcoeffs+1))
#define FREQ  (((here->ISRCfunctionOrder >=3) && (*(here->ISRCcoeffs+2)))? \
    (*(here->ISRCcoeffs+2)):(1/ckt->CKTfinalTime))
#define TD    ((here->ISRCfunctionOrder >=4)?(*(here->ISRCcoeffs+3)):(0.0))
#define THETA ((here->ISRCfunctionOrder >=5)?(*(here->ISRCcoeffs+4)):(0.0))
#define PHASE ((here->ISRCfunctionOrder >=6)?(*(here->ISRCcoeffs+5)):(0.0))

                    double phase;

                    /* offset time by delay and limit to zero */
                    time -= TD;
                    if(time < 0.0)
                        time = 0.0;

                    /* compute phase in radians */ 
                    phase = PHASE * PI / 180.0;

                    /* compute waveform value */
                    value = VO + VA * sin(FREQ*time * 2 * PI + phase) * 
                            exp(-time*THETA);
#undef VO
#undef VA
#undef FREQ
#undef TD
#undef THETA
#undef PHASE
                }
                break;

/* gtri - end - wbk - add PHASE parameter */

                case EXP: {
                    double td1;
                    double td2;
#define V1 (*(here->ISRCcoeffs))
#define V2 (*(here->ISRCcoeffs+1))
#define TD1 ((here->ISRCfunctionOrder >=3)?(*(here->ISRCcoeffs+2)):\
    ckt->CKTstep)
#define TAU1 (((here->ISRCfunctionOrder >=4) && (*(here->ISRCcoeffs+3)))? \
    (*(here->ISRCcoeffs+3)):ckt->CKTstep)
#define TD2 (((here->ISRCfunctionOrder >=5) && (*(here->ISRCcoeffs+4)))? \
    (*(here->ISRCcoeffs+4)):TD1+ckt->CKTstep)
#define TAU2 (((here->ISRCfunctionOrder >=6) && (*(here->ISRCcoeffs+5)))? \
    (*(here->ISRCcoeffs+5)):ckt->CKTstep)
                    td1 = TD1;
                    td2 = TD2;
                    if(time <= td1)  {
                        value = V1;
                    } else if (time <= td2) {
                        value = V1 + (V2-V1)*(1-exp(-(time-td1)/TAU1));
                    } else {
                        value = V1 + (V2-V1)*(1-exp(-(time-td1)/TAU1)) +
                                     (V1-V2)*(1-exp(-(time-td2)/TAU2)) ;
                    }
#undef V1
#undef V2
#undef TD1
#undef TAU1
#undef TD2
#undef TAU2
                }
                break;

/* gtri - begin - wbk - add PHASE parameters */

                case SFFM:{
#define VO (*(here->ISRCcoeffs))
#define VA (*(here->ISRCcoeffs+1))
#define FC (((here->ISRCfunctionOrder >=3) && (*(here->ISRCcoeffs+2)))? \
    (*(here->ISRCcoeffs+2)):(1/ckt->CKTfinalTime))
#define MDI ((here->ISRCfunctionOrder>=4)?(*(here->ISRCcoeffs+3)):0.0)
#define FS (((here->ISRCfunctionOrder >=5) && (*(here->ISRCcoeffs+4)))? \
    (*(here->ISRCcoeffs+4)):(1/ckt->CKTfinalTime))
#define PHASEC ((here->ISRCfunctionOrder>=6)?(*(here->ISRCcoeffs+5)):0.0)
#define PHASES ((here->ISRCfunctionOrder>=7)?(*(here->ISRCcoeffs+6)):0.0)

                    double phasec;
                    double phases;

                    /* compute phases in radians */
                    phasec = PHASEC * PI / 180.0;
                    phases = PHASES * PI / 180.0;

                    /* compute waveform value */
                    value = VO + VA * 
                        sin((2 * PI * FC * time + phasec) +
                        MDI * sin(2 * PI * FS * time + phases));
#undef VO
#undef VA
#undef FC
#undef MDI
#undef FS
#undef PHASEC
#undef PHASES
                }
                break;

/* gtri - end - wbk - add PHASE parameters */

                default:
                    value = here->ISRCdcValue;
                    break;
                case PWL: {
                    register int i;
                    if(time< *(here->ISRCcoeffs)) {
                        value = *(here->ISRCcoeffs + 1) ;
                        break;
                    }
                    for(i=0;i<=(here->ISRCfunctionOrder/2)-1;i++) {
                        if((*(here->ISRCcoeffs+2*i)==time)) {
                            value = *(here->ISRCcoeffs+2*i+1);
                            goto loadDone;
                        }
                        if((*(here->ISRCcoeffs+2*i)<time) &&
                                (*(here->ISRCcoeffs+2*(i+1)) >time)) {
                            value = *(here->ISRCcoeffs+2*i+1) +
                                (((time-*(here->ISRCcoeffs+2*i))/
                                (*(here->ISRCcoeffs+2*(i+1)) - 
                                 *(here->ISRCcoeffs+2*i))) *
                                (*(here->ISRCcoeffs+2*i+3) - 
                                 *(here->ISRCcoeffs+2*i+1)));
                            goto loadDone;
                        }
                    }
                    value = *(here->ISRCcoeffs+ here->ISRCfunctionOrder-1) ;
                    break;
                }
                }
            }
loadDone:
            value *= ckt->CKTsrcFact;

/* gtri - begin - wbk - modify for supply ramping option */
            value *= cm_analog_ramp_factor();
/* gtri - begin - wbk - modify for supply ramping option */

            *(ckt->CKTrhs + (here->ISRCposNode)) += value;
            *(ckt->CKTrhs + (here->ISRCnegNode)) -= value;

/* gtri - end - wbk - modify to process srcFact, etc. for all sources */

/* gtri - begin - wbk - record value so it can be output if requested */
            here->ISRCcurrent = value;
/* gtri - end   - wbk - record value so it can be output if requested */

        }
    }
    return(OK);
}
