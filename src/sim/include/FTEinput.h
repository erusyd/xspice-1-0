
/*
    $Header: //pepper/atesse_spice/spice3/include/RCS/FTEinput.h,v 1.1 91/04/02 11:25:56 bill Exp $
    Copyright 1988 Jeffrey M. Hsu

    Defs to use the Input routine.

    char_option is used by the lexer and the command interpreter
      response.reply contains a pointer to a character
    button_option is used by the menu system and the help package
      response.reply contains the button number
    click_option is used in the X10 version for the hardcopy command
      response.reply is the associated graph structure
*/

#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>
#include "FTEgraph.h"

typedef enum {
    error_option,       /* a reply option only */
    button_option,
    char_option,
    click_option,
} OPTION;

typedef struct request {
    OPTION option;
    FILE *fp;
} REQUEST;

typedef struct response {
    OPTION option;
    union {
      int ch;
      GRAPH *graph;
      int button;
    } reply;
} RESPONSE;

#endif  /* notdef _INPUT_H_ */
