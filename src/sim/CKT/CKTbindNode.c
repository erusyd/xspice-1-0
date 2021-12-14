/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /* CKTbindNode
     *  bind a node of the specified device of the given type to its place
     *  in the specified circuit.
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "SMPdefs.h"
#include "CKTdefs.h"
#include "util.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("CKTbindNode.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:12 $")

extern SPICEdev *DEVices[];

/*ARGSUSED*/
int
CKTbindNode(ckt,fast,term,node)
    GENERIC *ckt;
    GENERIC *fast;
    int term;
    GENERIC *node;
{
    int mappednode;
    register int type = ((GENinstance *)fast)->GENmodPtr->GENmodType;

    mappednode = ((CKTnode *)node)->number;

    if((*DEVices[type]).DEVpublic.terms >= term && term >0 ) {
        switch(term) {
            default: return(E_NOTERM);
            case 1:
                ((GENinstance *)fast)->GENnode1 = mappednode;
                break;
            case 2:
                ((GENinstance *)fast)->GENnode2 = mappednode;
                break;
            case 3:
                ((GENinstance *)fast)->GENnode3 = mappednode;
                break;
            case 4:
                ((GENinstance *)fast)->GENnode4 = mappednode;
                break;
            case 5:
                ((GENinstance *)fast)->GENnode5 = mappednode;
                break;
        }
        return(OK);
    } else {
        return(E_NOTERM);
    }
}
