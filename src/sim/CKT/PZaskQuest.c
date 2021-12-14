/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "PZdefs.h"
#include "CKTdefs.h"
#include "suffix.h"

RCSID("PZaskQuest.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:45 $")

/* ARGSUSED */
int 
PZaskQuest(ckt,anal,which,value)
    CKTcircuit *ckt;
    GENERIC *anal;
    int which;
    IFvalue *value;
{
    switch(which) {

    case PZ_NODEI:
        value->nValue = (IFnode)CKTnum2nod(ckt,((PZAN*)anal)->PZnodeI);
        break;

    case PZ_NODEG:
        value->nValue = (IFnode)CKTnum2nod(ckt,((PZAN*)anal)->PZnodeG);
        break;

    case PZ_NODEJ:
        value->nValue = (IFnode)CKTnum2nod(ckt,((PZAN*)anal)->PZnodeJ);
        break;

    case PZ_NODEK:
        value->nValue = (IFnode)CKTnum2nod(ckt,((PZAN*)anal)->PZnodeK);
        break;

    case PZ_V:
        if( ((PZAN*)anal)->PZflagVI == 0) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    case PZ_I:
        if( ((PZAN*)anal)->PZflagVI == 1) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    case PZ_POL:
        if( ((PZAN*)anal)->PZflagVI == 1) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    case PZ_ZER:
        if( ((PZAN*)anal)->PZflagVI == 2) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    case PZ_PZ:
        if( ((PZAN*)anal)->PZflagVI == 3) {
            value->iValue=1;
        } else {
            value->iValue=0;
        }
        break;

    default:
        return(E_BADPARM);
    }
    return(OK);
}

