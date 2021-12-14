/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "CKTdefs.h"
#include "VSRCdefs.h"
#include "TRANdefs.h"
#include "SPerror.h"

/* gtri - begin - wbk - modify for supply ramping option */
#include "CMproto.h"
/* gtri - end   - wbk - modify for supply ramping option */

#include "suffix.h"

RCSID("VSRCload.c $Revision: 1.4 $ on $Date: 92/01/05 12:48:48 $")

int
VSRCload(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
        /* actually load the current voltage value into the 
         * sparse matrix previously provided 
         */
{
    register VSRCmodel *model = (VSRCmodel *)inModel;
    register VSRCinstance *here;
    double time;

/* gtri - begin - wbk - modify to process srcFact, etc. for all sources */
    double value;
/* gtri - end - wbk - modify to process srcFact, etc. for all sources */

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VSRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VSRCinstances; here != NULL ;
                here=here->VSRCnextInstance) {
            
            *(here->VSRCposIbrptr) += 1.0 ;
            *(here->VSRCnegIbrptr) -= 1.0 ;
            *(here->VSRCibrPosptr) += 1.0 ;
            *(here->VSRCibrNegptr) -= 1.0 ;

/* gtri - begin - wbk - modify to process srcFact, etc. for all sources */

            if( (ckt->CKTmode & (MODEDCOP | MODEDCTRANCURVE)) &&
                    here->VSRCdcGiven ) {
                /* load using DC value */
                value = here->VSRCdcValue;
            } else {
                if(ckt->CKTmode & (MODEDC)) {
                    time = 0;
                } else {
                    time = ckt->CKTtime;
                }
                /* use transient function */
                switch(here->VSRCfunctionType) {

/* gtri - begin - wbk - add PHASE parameter */

                case PULSE: {
#define V1 (*(here->VSRCcoeffs))
#define V2 (*(here->VSRCcoeffs+1))
#define TD ((here->VSRCfunctionOrder >=3)?(*(here->VSRCcoeffs+2)):\
    (0.0))
#define TR (((here->VSRCfunctionOrder >=4) && (*(here->VSRCcoeffs+3)))? \
    (*(here->VSRCcoeffs+3)):(ckt->CKTstep))
#define TF (((here->VSRCfunctionOrder >=5) && (*(here->VSRCcoeffs+4)))? \
    (*(here->VSRCcoeffs+4)):(ckt->CKTstep))
#define PW (((here->VSRCfunctionOrder >=6) && (*(here->VSRCcoeffs+5)))? \
    (*(here->VSRCcoeffs+5)):(ckt->CKTfinalTime))
#define PER (((here->VSRCfunctionOrder>=7) && (*(here->VSRCcoeffs+6)))? \
    (*(here->VSRCcoeffs+6)):(ckt->CKTfinalTime))
#define PHASE (((here->VSRCfunctionOrder>=8) && (*(here->VSRCcoeffs+7)))? \
    (*(here->VSRCcoeffs+7)):(0.0))
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
#define VO    (*(here->VSRCcoeffs))
#define VA    (*(here->VSRCcoeffs+1))
#define FREQ  (((here->VSRCfunctionOrder >=3) && (*(here->VSRCcoeffs+2)))? \
    (*(here->VSRCcoeffs+2)):(1/ckt->CKTfinalTime))
#define TD    ((here->VSRCfunctionOrder >=4)?(*(here->VSRCcoeffs+3)):(0.0))
#define THETA ((here->VSRCfunctionOrder >=5)?(*(here->VSRCcoeffs+4)):(0.0))
#define PHASE ((here->VSRCfunctionOrder >=6)?(*(here->VSRCcoeffs+5)):(0.0))

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
#define V1 (*(here->VSRCcoeffs))
#define V2 (*(here->VSRCcoeffs+1))
#define TD1 ((here->VSRCfunctionOrder >=3)?(*(here->VSRCcoeffs+2)):\
    ckt->CKTstep)
#define TAU1 (((here->VSRCfunctionOrder >=4) && (*(here->VSRCcoeffs+3)))? \
    (*(here->VSRCcoeffs+3)):ckt->CKTstep)
#define TD2 (((here->VSRCfunctionOrder >=5) && (*(here->VSRCcoeffs+4)))? \
    (*(here->VSRCcoeffs+4)):TD1+ckt->CKTstep)
#define TAU2 (((here->VSRCfunctionOrder >=6) && (*(here->VSRCcoeffs+5)))? \
    (*(here->VSRCcoeffs+5)):ckt->CKTstep)
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
#define VO (*(here->VSRCcoeffs))
#define VA (*(here->VSRCcoeffs+1))
#define FC (((here->VSRCfunctionOrder >=3) && (*(here->VSRCcoeffs+2)))? \
    (*(here->VSRCcoeffs+2)):(1/ckt->CKTfinalTime))
#define MDI ((here->VSRCfunctionOrder>=4)?(*(here->VSRCcoeffs+3)):0.0)
#define FS (((here->VSRCfunctionOrder >=5) && (*(here->VSRCcoeffs+4)))? \
    (*(here->VSRCcoeffs+4)):(1/ckt->CKTfinalTime))
#define PHASEC ((here->VSRCfunctionOrder>=6)?(*(here->VSRCcoeffs+5)):0.0)
#define PHASES ((here->VSRCfunctionOrder>=7)?(*(here->VSRCcoeffs+6)):0.0)

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
                    value = here->VSRCdcValue;
                    break;
                case PWL: {
                    register int i;
                    if(time< *(here->VSRCcoeffs)) {
                        value = *(here->VSRCcoeffs + 1) ;
                        break;
                    }
                    for(i=0;i<=(here->VSRCfunctionOrder/2)-1;i++) {
                        if((*(here->VSRCcoeffs+2*i)==time)) {
                            value = *(here->VSRCcoeffs+2*i+1);
                            goto loadDone;
                        }
                        if((*(here->VSRCcoeffs+2*i)<time) &&
                                (*(here->VSRCcoeffs+2*(i+1)) >time)) {
                            value = *(here->VSRCcoeffs+2*i+1) +
                                (((time-*(here->VSRCcoeffs+2*i))/
                                (*(here->VSRCcoeffs+2*(i+1)) - 
                                 *(here->VSRCcoeffs+2*i))) *
                                (*(here->VSRCcoeffs+2*i+3) - 
                                 *(here->VSRCcoeffs+2*i+1)));
                            goto loadDone;
                        }
                    }
                    value = *(here->VSRCcoeffs+ here->VSRCfunctionOrder-1) ;
                    break;
                }
                }
            }
loadDone:
            value *= ckt->CKTsrcFact;

/* gtri - begin - wbk - modify for supply ramping option */
            value *= cm_analog_ramp_factor();
/* gtri - begin - wbk - modify for supply ramping option */

            *(ckt->CKTrhs + (here->VSRCbranch)) += value;

/* gtri - end - wbk - modify to process srcFact, etc. for all sources */


        }
    }
    return(OK);
}
