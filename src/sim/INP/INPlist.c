/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* INPlist(file,deck,type)
     *  provide an input listing on the specified file of the given
     *  card deck.  The listing should be of either PHYSICAL or LOGICAL
     *  lines as specified by the type parameter.
     */

#include "prefix.h"
#include <stdio.h>
#include "INPdefs.h"
#include "util.h"
#include "suffix.h"

RCSID("INPlist.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:55 $")

void
INPlist(file,deck,type)
    FILE *file;
    card *deck;
    int type;
{

    card *here;
    card *there;

    if(type == LOGICAL) {
        for(here = deck;here != NULL;here = here->nextcard) {
            fprintf(file,"%6d : %s\n",here->linenum, here->line);
            if(here->error != (char *)NULL) {
                fprintf(file,"%s",here->error);
            }
        }
    } else if (type == PHYSICAL) {
        for(here = deck;here != NULL;here = here->nextcard) {
            if(here->actualLine == (card *)NULL) {
                fprintf(file,"%6d : %s\n",here->linenum,here->line);
                if(here->error != (char *)NULL) {
                    fprintf(file,"%s",here->error);
                }
            } else {
                for(there = here->actualLine;there != NULL;
                        there=there->nextcard) {
                    fprintf(file,"%6d : %s\n",there->linenum, there->line);
                    if(there->error != (char *)NULL) {
                        fprintf(file,"%s",there->error);
                    }
                }
            }
        }
    }
}
                    
                
