/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CONST.h"
#include "DEVdefs.h"
#include "IFsim.h"
#include "CKTdefs.h"
#include "DIOdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("DIOask.c $Revision: 1.1 $ on $Date: 90/10/11 12:42:33 $")

/* ARGSUSED */
int
DIOask (ckt,inst,which,value,select) 
    CKTcircuit *ckt;
    GENinstance *inst;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    DIOinstance *here = (DIOinstance*)inst;
    double vr;
    double vi;
    double sr;
    double si;
    double vm;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {
        case DIO_OFF:
            value->iValue = here->DIOoff;
            return(OK);
        case DIO_IC:
            value->rValue = here->DIOinitCond;
            return(OK);
        case DIO_AREA:
            value->rValue = here->DIOarea;
            return(OK);
        case DIO_TEMP:
            value->rValue = here->DIOtemp-CONSTCtoK;
            return(OK);
        case DIO_VOLTAGE:
            value->rValue = *(ckt->CKTstate0+here->DIOvoltage);
            return(OK);
        case DIO_CURRENT:
            value->rValue = *(ckt->CKTstate0+here->DIOcurrent);
            return(OK);
        case DIO_CHARGE: 
            value->rValue = *(ckt->CKTstate0+here->DIOcapCharge);
            return(OK);
        case DIO_CAPCUR:
            value->rValue = *(ckt->CKTstate0+here->DIOcapCurrent);
            return(OK);
        case DIO_CONDUCT:
            value->rValue = *(ckt->CKTstate0+here->DIOconduct);
            return(OK);
        case DIO_POWER :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "DIOask";
                strcpy(errMsg,msg);
                return(E_ASKPOWER);
            } else {
                value->rValue = *(ckt->CKTstate0 + here->DIOcurrent) *
                        *(ckt->CKTstate0 + here->DIOvoltage);
            }
            return(OK);
        case DIO_QUEST_SENS_DC:
            if(ckt->CKTsenInfo){
                value->rValue = *(ckt->CKTsenInfo->SEN_Sap[select->iValue + 1]+
                here->DIOsenParmNo);
            }
            return(OK);
        case DIO_QUEST_SENS_REAL:
            if(ckt->CKTsenInfo){
                value->rValue = *(ckt->CKTsenInfo->SEN_RHS[select->iValue + 1]+
                here->DIOsenParmNo);
            }
            return(OK);
        case DIO_QUEST_SENS_IMAG:
            if(ckt->CKTsenInfo){
                value->rValue = *(ckt->CKTsenInfo->SEN_iRHS[select->iValue + 1]+
                here->DIOsenParmNo);
            }
            return(OK);
        case DIO_QUEST_SENS_MAG:
            if(ckt->CKTsenInfo){
                vr = *(ckt->CKTrhsOld + select->iValue + 1); 
                vi = *(ckt->CKTirhsOld + select->iValue + 1); 
                vm = sqrt(vr*vr + vi*vi);
                if(vm == 0){
                    value->rValue = 0;
                    return(OK);
                }
                sr = *(ckt->CKTsenInfo->SEN_RHS[select->iValue + 1]+
                        here->DIOsenParmNo);
                si = *(ckt->CKTsenInfo->SEN_iRHS[select->iValue + 1]+
                        here->DIOsenParmNo);
                value->rValue = (vr * sr + vi * si)/vm;
            }
            return(OK);
        case DIO_QUEST_SENS_PH:
            if(ckt->CKTsenInfo){
                vr = *(ckt->CKTrhsOld + select->iValue + 1); 
                vi = *(ckt->CKTirhsOld + select->iValue + 1); 
                vm = vr*vr + vi*vi;
                if(vm == 0){
                    value->rValue = 0;
                    return(OK);
                }
                sr = *(ckt->CKTsenInfo->SEN_RHS[select->iValue + 1]+
                        here->DIOsenParmNo);
                si = *(ckt->CKTsenInfo->SEN_iRHS[select->iValue + 1]+
                        here->DIOsenParmNo);

                value->rValue = (vr * si - vi * sr)/vm;
            }
            return(OK);
        case DIO_QUEST_SENS_CPLX:
            if(ckt->CKTsenInfo){
                value->cValue.real= 
                        *(ckt->CKTsenInfo->SEN_RHS[select->iValue + 1]+
                        here->DIOsenParmNo);
                value->cValue.imag= 
                        *(ckt->CKTsenInfo->SEN_iRHS[select->iValue + 1]+
                        here->DIOsenParmNo);
            }
            return(OK);
        default:
            return(E_BADPARM);
        }
}  

