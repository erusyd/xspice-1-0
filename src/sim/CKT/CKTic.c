/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "CKTdefs.h"
#include "SMPdefs.h"
#include "util.h"
#include "SPerror.h"
#include "DEVdefs.h"
#include "suffix.h"

RCSID("CKTic.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:37 $")

extern SPICEdev *DEVices[];

int
CKTic(ckt)
    CKTcircuit *ckt;
{
    int error;
    int size;
    int i;
    CKTnode *node;

    size = SMPmatSize(ckt->CKTmatrix);
    for (i=0;i<=size;i++) {
        *(ckt->CKTrhs+i)=0;
    }

    for(node = ckt->CKTnodes;node != NULL; node = node->next) {
        if(node->nsGiven) {
            node->ptr = SMPmakeElt(ckt->CKTmatrix,node->number,node->number);
            if(node->ptr == (double *)NULL) return(E_NOMEM);
            ckt->CKThadNodeset = 1;
            *(ckt->CKTrhs+node->number) = node->nodeset;
        }
        if(node->icGiven) {
            if(! ( node->ptr)) {
                node->ptr = SMPmakeElt(ckt->CKTmatrix,node->number,
                        node->number);
                if(node->ptr == (double *)NULL) return(E_NOMEM);
            }
            *(ckt->CKTrhs+node->number) = node->ic;
        }
    }

    if(ckt->CKTmode & MODEUIC) {
        for (i=0;i<DEVmaxnum;i++) {
            if( ((*DEVices[i]).DEVsetic != NULL) && (ckt->CKThead[i] != NULL) ){
                error = (*((*DEVices[i]).DEVsetic))(ckt->CKThead[i],ckt);
                if(error) return(error);
            }
        }
    }

    return(OK);
}
