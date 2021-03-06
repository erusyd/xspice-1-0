/*============================================================================
FILE    MIFutil.c

MEMBER OF process XSPICE

Copyright 1991
Georgia Tech Research Corporation
Atlanta, Georgia 30332
All Rights Reserved

PROJECT A-8503

AUTHORS

    9/12/91  Bill Kuhn

MODIFICATIONS

    <date> <person name> <nature of modifications>

SUMMARY

    This file contains various utility routines used by the MIF package.

INTERFACES

    MIFgettok()
    MIFget_token()
    MIFget_cntl_src_type()
    MIFcopy()

REFERENCED FILES

    None.

NON-STANDARD FEATURES

    None.

============================================================================*/

#include "prefix.h"

#include "util.h"
#include "CPstd.h"
#include <string.h>
#include "MIFtypes.h"
#include "MIFproto.h"

#include "suffix.h"




/*

MIFgettok

Get the next token from the input string.  The input string pointer
is advanced to the following token and the token from the input
string is copied to malloced storage and a pointer to that storage
is returned.  The original input string is undisturbed.

*/

char  *MIFgettok(char **s)
{

    char    *buf;       /* temporary storage to copy token into */
    char    *ret_str;   /* storage for returned string */

    int     i;

    /* allocate space big enough for the whole string */

    buf = (void *) MALLOC(strlen(*s) + 1);

    /* skip over any white space */

    while(isspace(**s) || (**s == '=') ||
          (**s == '(') || (**s == ')') || (**s == ','))
        (*s)++;

    /* isolate the next token */

    switch(**s) {

    case '\0':
        FREE(buf);
        return(NULL);

    case '<':
    case '>':
    case '[':
    case ']':
    case '~':
    case '%':
        buf[0] = **s;
        buf[1] = '\0';
        (*s)++;
        break;

    default:
        i = 0;
        /* if first character is a quote, read until the closing */
        /* quote, or the end of string, discarding the quotes */
        if(**s == '"') {
            (*s)++;
            while( (**s != '\0') && (**s != '"') ) {
                buf[i] = **s;
                i++;
                (*s)++;
            }
            if(**s == '"')
                (*s)++;
        }
        /* else, read until the next delimiter */
        else {
            while( (**s != '\0') &&
                   (! ( isspace(**s) || (**s == '=') || (**s == '%') ||
                        (**s == '(') || (**s == ')') || (**s == ',') ||
                        (**s == '[') || (**s == ']') ||
                        (**s == '<') || (**s == '>') || (**s == '~')
                   )  )  ) {
                buf[i] = **s;
                i++;
                (*s)++;
            }
        }

        buf[i] = '\0';
        break;
    }

    /* skip over white space up to next token */

    while(isspace(**s) || (**s == '=') ||
          (**s == '(') || (**s == ')') || (**s == ','))
        (*s)++;

    /* make a copy using only the space needed by the string length */

    ret_str = copy(buf);
    FREE(buf);

    return(ret_str);
}




/*

MIFget_token

Get the next token from the input string together with its type.
The input string pointer
is advanced to the following token and the token from the input
string is copied to malloced storage and a pointer to that storage
is returned.  The original input string is undisturbed.

*/

char  *MIFget_token(
    char             **s,     /* The text line to get the token from */
    Mif_Token_Type_t *type)   /* The type of token found */
{

    char    *ret_str;   /* storage for returned string */

    /* get the token from the input line */

    ret_str = MIFgettok(s);


    /* if no next token, return */

    if(ret_str == NULL) {
        *type = MIF_NO_TOK;
        return(NULL);
    }

    /* else, determine and return token type */

    switch(*ret_str) {

    case '[':
        *type = MIF_LARRAY_TOK;
        break;

    case ']':
        *type = MIF_RARRAY_TOK;
        break;

    case '<':
        *type = MIF_LCOMPLEX_TOK;
        break;

    case '>':
        *type = MIF_RCOMPLEX_TOK;
        break;

    case '%':
        *type = MIF_PERCENT_TOK;
        break;

    case '~':
        *type = MIF_TILDE_TOK;
        break;

    default:
        if(strcmp(ret_str, "null") == 0)
            *type = MIF_NULL_TOK;
        else
            *type = MIF_STRING_TOK;
        break;

    }

    return(ret_str);
}



/*
MIFget_cntl_src_type

This function takes an input connection/port type and an output
connection/port type (MIF_VOLTAGE, MIF_CURRENT, etc.) and maps
this pair to one of the four controlled source types used in
SPICE (VCVS, VCIS, ICVS, ICIS).
*/


Mif_Cntl_Src_Type_t MIFget_cntl_src_type(
    Mif_Port_Type_t in_port_type,         /* The type of the input port */
    Mif_Port_Type_t out_port_type)        /* The type of the output port */
{

    switch(in_port_type) {

    case MIF_VOLTAGE:
    case MIF_DIFF_VOLTAGE:
    case MIF_CONDUCTANCE:
    case MIF_DIFF_CONDUCTANCE:

        switch(out_port_type) {

        case MIF_VOLTAGE:
        case MIF_DIFF_VOLTAGE:
        case MIF_RESISTANCE:
        case MIF_DIFF_RESISTANCE:
            return(MIF_VCVS);
            break;

        case MIF_CURRENT:
        case MIF_DIFF_CURRENT:
        case MIF_CONDUCTANCE:
        case MIF_DIFF_CONDUCTANCE:
            return(MIF_VCIS);
            break;

        default:
            break;

        }
        break;

    case MIF_CURRENT:
    case MIF_DIFF_CURRENT:
    case MIF_VSOURCE_CURRENT:
    case MIF_RESISTANCE:
    case MIF_DIFF_RESISTANCE:

        switch(out_port_type) {

        case MIF_VOLTAGE:
        case MIF_DIFF_VOLTAGE:
        case MIF_RESISTANCE:
        case MIF_DIFF_RESISTANCE:
            return(MIF_ICVS);
            break;

        case MIF_CURRENT:
        case MIF_DIFF_CURRENT:
        case MIF_CONDUCTANCE:
        case MIF_DIFF_CONDUCTANCE:
            return(MIF_ICIS);
            break;

        default:
            break;

        }
        break;

    default:
        break;

    }

    return(-1);
}


/*
MIFcopy

This function allocates a new copy of a string.
*/

char *MIFcopy(char *str)
{
    char *temp;

    /* Allocate space for the string and then copy it */
    temp = MALLOC(strlen(str) + 1);
    strcpy(temp,str);

    return(temp);
}
