/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* get input token from 'line', 
     *  and return a pointer to it in 'token'
     */

#include "prefix.h"
#include <string.h>
#include <stdio.h>
#include "util.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "INPdefs.h"
#include "suffix.h"

RCSID("INPgetTok.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:50 $")

int
INPgetTok(line,token,gobble)
    char ** line;
    char ** token;
	int gobble; /* eat non-whitespace trash AFTER token? */
{
    char * point;
    /* scan along throwing away garbage characters */
    for(point = *line;*point != '\0' ; point++ ) {
        if(*point == ' ') continue;
        if(*point == '\t') continue;
        if(*point == '=') continue;
        if(*point == '(') continue;
        if(*point == ')') continue;
        if(*point == ',') continue;
        break;
    }
    /* mark beginning of token */
    *line = point;
    /* now find all good characters */
    for(point = *line;*point!='\0';point++) {
        if(*point == ' ') break;
        if(*point == '\t') break;
        if(*point == '=') break;
        if(*point == '(') break;
        if(*point == ')') break;
        if(*point == ',') break;
    }
    *token=(char *)MALLOC(1+point-*line);
    if(!*token) return(E_NOMEM);
    (void) strncpy(*token,*line,point-*line);
    *(*token + (point-*line)) = '\0';
    *line = point;
    /* gobble garbage to next token */
    for( ;**line != '\0' ; (*line)++ ) {
        if(**line == ' ') continue;
        if(**line == '\t') continue;
        if((**line == '=') && gobble) continue;
        if((**line == /* (match */')') && gobble) continue;
        if((**line == ',') && gobble) continue;
#ifdef notdef
    /* This is the wrong thing to do for expression-valued parameters. */
        if((**line == '(' /* match) */) && gobble) continue;
#endif
        break;
    }
    /*printf("found token (%s) and rest of line (%s)\n",*token,*line);*/
    return(OK);
}
