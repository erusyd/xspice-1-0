/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* CKTinst2Node
     *  get the name and node pointer for a node given a device it is
     * bound to and the terminal of the device.
     */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "SPerror.h"
#include "CKTdefs.h"
#include "GENdefs.h"
#include "DEVdefs.h"
#include "suffix.h"

RCSID("CKTinst2Node.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:39 $")

extern SPICEdev *DEVices[];

int
CKTinst2Node(ckt,instPtr,terminal,node,nodeName)
    GENERIC *ckt;
    GENERIC *instPtr;
    int terminal;
    GENERIC **node;
    IFuid *nodeName;

{
    int nodenum;
    register int type;
    CKTnode *here;

    type = ((GENinstance *)instPtr)->GENmodPtr->GENmodType;

    if((*DEVices[type]).DEVpublic.terms >= terminal && terminal >0 ) {
        switch(terminal) {
            default: return(E_NOTERM);
            case 1:
                nodenum = ((GENinstance *)instPtr)->GENnode1;
                break;
            case 2:
                nodenum = ((GENinstance *)instPtr)->GENnode2;
                break;
            case 3:
                nodenum = ((GENinstance *)instPtr)->GENnode3;
                break;
            case 4:
                nodenum = ((GENinstance *)instPtr)->GENnode4;
                break;
            case 5:
                nodenum = ((GENinstance *)instPtr)->GENnode5;
                break;
        }
        /* ok, now we know its number, so we just have to find it.*/
        for(here = ((CKTcircuit*)ckt)->CKTnodes;here;here = here->next) {
            if(here->number == nodenum) {
                /* found it */
                *node = (GENERIC*) here;
                *nodeName = here->name;
                return(OK);
            }
        }
        return(E_NOTFOUND);
    } else {
        return(E_NOTERM);
    }
}
