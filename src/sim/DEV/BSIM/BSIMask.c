/*
 * Copyright (c) 1988 Hong J. Park
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "IFsim.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "BSIMdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("BSIMask.c $Revision: 1.1 $ on $Date: 90/10/11 12:41:01 $")

/*ARGSUSED*/
int
BSIMask(ckt,inst,which,value,select)
    CKTcircuit *ckt;
    GENinstance *inst;
    int which;
    IFvalue *value;
    IFvalue *select;
{
    BSIMinstance *here = (BSIMinstance*)inst;

    switch(which) {
        case BSIM_L:
            value->rValue = here->BSIMl;
            return(OK);
        case BSIM_W:
            value->rValue = here->BSIMw;
            return(OK);
        case BSIM_AS:
            value->rValue = here->BSIMsourceArea;
            return(OK);
        case BSIM_AD:
            value->rValue = here->BSIMdrainArea;
            return(OK);
        case BSIM_PS:
            value->rValue = here->BSIMsourcePerimeter;
            return(OK);
        case BSIM_PD:
            value->rValue = here->BSIMdrainPerimeter;
            return(OK);
        case BSIM_NRS:
            value->rValue = here->BSIMsourceSquares;
            return(OK);
        case BSIM_NRD:
            value->rValue = here->BSIMdrainSquares;
            return(OK);
        case BSIM_OFF:
            value->rValue = here->BSIMoff;
            return(OK);
        case BSIM_IC_VBS:
            value->rValue = here->BSIMicVBS;
            return(OK);
        case BSIM_IC_VDS:
            value->rValue = here->BSIMicVDS;
            return(OK);
        case BSIM_IC_VGS:
            value->rValue = here->BSIMicVGS;
            return(OK);
        case BSIM_DNODE:
            value->iValue = here->BSIMdNode;
            return(OK);
        case BSIM_GNODE:
            value->iValue = here->BSIMgNode;
            return(OK);
        case BSIM_SNODE:
            value->iValue = here->BSIMsNode;
            return(OK);
        case BSIM_BNODE:
            value->iValue = here->BSIMbNode;
            return(OK);
        case BSIM_DNODEPRIME:
            value->iValue = here->BSIMdNodePrime;
            return(OK);
        case BSIM_SNODEPRIME:
            value->iValue = here->BSIMsNodePrime;
            return(OK);
        case BSIM_SOURCECONDUCT:
            value->rValue = here->BSIMsourceConductance;
            return(OK);
        case BSIM_DRAINCONDUCT:
            value->rValue = here->BSIMdrainConductance;
            return(OK);
        case BSIM_VBD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMvbd);
            return(OK);
        case BSIM_VBS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMvbs);
            return(OK);
        case BSIM_VGS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMvgs);
            return(OK);
        case BSIM_VDS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMvds);
            return(OK);
        case BSIM_CD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcd); 
            return(OK);
        case BSIM_CBS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcbs); 
            return(OK);
        case BSIM_CBD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcbd); 
            return(OK);
        case BSIM_GM:
            value->rValue = *(ckt->CKTstate0 + here->BSIMgm); 
            return(OK);
        case BSIM_GDS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMgds); 
            return(OK);
        case BSIM_GMBS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMgmbs); 
            return(OK);
        case BSIM_GBD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMgbd); 
            return(OK);
        case BSIM_GBS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMgbs); 
            return(OK);
        case BSIM_QB:
            value->rValue = *(ckt->CKTstate0 + here->BSIMqb); 
            return(OK);
        case BSIM_CQB:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcqb); 
            return(OK);
        case BSIM_QG:
            value->rValue = *(ckt->CKTstate0 + here->BSIMqg); 
            return(OK);
        case BSIM_CQG:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcqg); 
            return(OK);
        case BSIM_QD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMqd); 
            return(OK);
        case BSIM_CQD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcqd); 
            return(OK);
        case BSIM_CGG:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcggb); 
            return(OK);
        case BSIM_CGD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcgdb); 
            return(OK);
        case BSIM_CGS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcgsb); 
            return(OK);
        case BSIM_CBG:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcbgb); 
            return(OK);
        case BSIM_CAPBD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcapbd); 
            return(OK);
        case BSIM_CQBD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcqbd); 
            return(OK);
        case BSIM_CAPBS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcapbs); 
            return(OK);
        case BSIM_CQBS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcqbs); 
            return(OK);
        case BSIM_CDG:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcdgb); 
            return(OK);
        case BSIM_CDD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcddb); 
            return(OK);
        case BSIM_CDS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMcdsb); 
            return(OK);
        case BSIM_VON:
            value->rValue = *(ckt->CKTstate0 + here->BSIMvono); 
            return(OK);
        case BSIM_QBS:
            value->rValue = *(ckt->CKTstate0 + here->BSIMqbs); 
            return(OK);
        case BSIM_QBD:
            value->rValue = *(ckt->CKTstate0 + here->BSIMqbd); 
            return(OK);
        default:
            return(E_BADPARM);
    }
    /* NOTREACHED */
}

