/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*  CKTbreakDump(ckt) - dump the breakpoint table associated with
     *  the given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("CKTbreakDump.c $Revision: 1.1 $ on $Date: 91/04/02 12:07:14 $")

void
CKTbreakDump(ckt)
    CKTcircuit *ckt;

{
    register int i;
    for(i=0;i<ckt->CKTbreakSize;i++) {
        (void)printf("breakpoint table entry %d is %g\n",i,*(ckt->CKTbreaks+i));
    }
}
