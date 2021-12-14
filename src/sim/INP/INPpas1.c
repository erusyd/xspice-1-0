/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "INPdefs.h"
#include "util.h"
#include "IFsim.h"
#include <string.h>
#include "suffix.h"

RCSID("INPpas1.c $Revision: 1.1 $ on $Date: 91/04/02 11:57:09 $")

void
INPpas1(ckt,deck,tab) 
    GENERIC *ckt;
    card *deck;
    INPtables *tab;

{
    card *current;
    char *INPdomodel();
    char *temp, *thisline;

    for(current = deck;current != NULL;current = current->nextcard) {
        /* SPICE-2 keys off of the first character of the line */
    thisline = current->line;
    while (*thisline && ((*thisline == ' ') || (*thisline == '\t')))
        thisline++;
        switch(*thisline) {

        case '.':
            /* a control or model card  - look at it more carefully */

            if(strncmp(thisline,".model",6)==0) { 
                /* a .MODEL card - have to analize it completely now */
                temp = INPdomodel(ckt,current,tab);
                current->error = INPerrCat(current->error,temp);

            }
            /* temporarily ignore all other '.' cards */

            break;
        default:
            /* for now, we do nothing with the other cards - just 
             * keep them in the list for pass 2 
             */
            break;

        }
    }
}
