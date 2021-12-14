
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:21 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/modify.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 */

#include "prefix.h"
#include "CPdefs.h"
#include "suffix.h"

char cp_chars[128];

static char *singlec = "<>;&";

/* Initialize stuff. */

void
cp_init()
{
    char *s, *getenv();

    bzero(cp_chars, 128);
    for (s = singlec; *s; s++)
        cp_chars[*s] = (CPC_BRR | CPC_BRL);
    cp_vset("history", VT_NUM, (char *) &cp_maxhistlength);

    cp_curin = stdin;
    cp_curout = stdout;
    cp_curerr = stderr;

    cp_ioreset();

    return;
}

