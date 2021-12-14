/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CPdefs.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
#include "suffix.h"

RCSID("INPapName.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:31 $")

int
INPapName(ckt,type,analPtr,parmname,value)
    GENERIC *ckt;
    int type;
    GENERIC *analPtr;
    char * parmname;
    IFvalue *value;
{
    int i;

    for(i=0;i<=ft_sim->analyses[type]->numParms;i++)
    if(strcmp(parmname,ft_sim->analyses[type]->analysisParms[i].keyword)==0) {
        return( (*(ft_sim->setAnalysisParm))(ckt,analPtr, ft_sim->
                analyses[type]->analysisParms[i].id,value,(IFvalue*)NULL) );
    }
    return(E_BADPARM);
}
