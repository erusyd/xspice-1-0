/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*  INPgetTitle(ckt,data)
     *      get the title card from the specified data deck and pass
     *      it through to SPICE-3.
     */

#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "INPdefs.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CPstd.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
#include "suffix.h"

RCSID("INPgetTitle.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:48 $")

int
INPgetTitle(ckt,data)
    GENERIC **ckt;
    card **data;

{
    int error;

    error = (*(ft_sim->newCircuit))(ckt);
    if(error) return(error);
    *data = (*data)->nextcard;
    return(OK);
}

