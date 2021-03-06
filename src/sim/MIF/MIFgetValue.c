/*============================================================================
FILE    MIFgetValue.c

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

    This file contains the function called to read parameter values from a
    .model card.

INTERFACES

    MIFgetValue()

REFERENCED FILES

    None.

NON-STANDARD FEATURES

    None.

============================================================================*/

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "util.h"
#include "INPdefs.h"
#include "INPparseTree.h"

/* #include <stdlib.h> */
#include <errno.h>

#include "MIFproto.h"
#include "MIFparse.h"
#include "MIFdefs.h"
#include "MIFcmdat.h"

#include "suffix.h"



static int MIFget_boolean(char *token, char **err);

static int MIFget_integer(char *token, char **err);

static double MIFget_real(char *token, char **err);

static char *MIFget_string(char *token, char **err);

static IFcomplex MIFget_complex(char *token, Mif_Token_Type_t token_type,
                                char **line, char **err);



/*
MIFgetValue

This function gets a parameter value from the .model text line
into an IFvalue structure.  The parameter type is specified in
the argument list and is used to determine how to parse the text
on the .model line.  If the parameter is an array, the entire
array is parsed and placed in the IFvalue structure along with
the number of elements found.
*/


IFvalue *
MIFgetValue(ckt,line,type,tab,err)
    GENERIC   *ckt;       /* The circuit structure */
    char      **line;     /* The text line to read value from */
    int       type;       /* The type of data to read */
    INPtables *tab;       /* Unused */
    char      **err;      /* Error string text */
{
    static IFvalue val;

    int     btemp;
    int     itemp;
    double  rtemp;
    char    *stemp;
    IFcomplex   ctemp;

    char              *token;
    Mif_Token_Type_t  token_type;

    int     value_type;
    int     is_array;


    /* Mask off non-type bits */
    value_type = type & IF_VARTYPES;

    /* Setup array boolean */
    is_array = value_type & IF_VECTOR;


    /* initialize stuff if array */
    if(is_array) {
        token = MIFget_token(line, &token_type);
        if(token_type != MIF_LARRAY_TOK) {
            *err = "Array parameter expected - No array delimiter found";
            return(NULL);
        }
        val.v.numValue = 0;
        val.v.vec.iVec = (void *) MALLOC(1);   /* just so that realloc doesn't bomb */
    }


    /* now get the values into val */

    while(1) {

        token = MIFget_token(line, &token_type);

        /* exit if no more tokens */
        if(token_type == MIF_NO_TOK) {
            *err = "Unexpected end of model card";
            return(NULL);
        }

        /* exit if end of array found */
        if(is_array && (token_type == MIF_RARRAY_TOK)) {
            if(val.v.numValue == 0) {
                *err = "Array parameter must have at least one value";
                return(NULL);
            }
            break;
        }

        /* process the token to extract a value */
        switch(value_type) {

        case  IF_FLAG:
            val.iValue = MIFget_boolean(token, err);
            break;

        case  IF_INTEGER:
            val.iValue = MIFget_integer(token, err);
            break;

        case  IF_REAL:
            val.rValue = MIFget_real(token, err);
            break;

        case  IF_STRING:
            val.sValue = MIFget_string(token, err);
            break;

        case  IF_COMPLEX:
            val.cValue = MIFget_complex(token, token_type, line, err);
            break;


        case  IF_FLAGVEC:
            btemp = MIFget_boolean(token, err);
            val.v.vec.iVec = (void *) REALLOC(val.v.vec.iVec,
                                     (val.v.numValue + 1) * sizeof(int));
            val.v.vec.iVec[val.v.numValue] = btemp;
            val.v.numValue++;
            break;

        case  IF_INTVEC:
            itemp = MIFget_integer(token, err);
            val.v.vec.iVec = (void *) REALLOC(val.v.vec.iVec,
                                     (val.v.numValue + 1) * sizeof(int));
            val.v.vec.iVec[val.v.numValue] = itemp;
            val.v.numValue++;
            break;

        case  IF_REALVEC:
            rtemp = MIFget_real(token, err);
            val.v.vec.rVec = (void *) REALLOC(val.v.vec.rVec,
                                     (val.v.numValue + 1) * sizeof(double));
            val.v.vec.rVec[val.v.numValue] = rtemp;
            val.v.numValue++;
            break;

        case  IF_STRINGVEC:
            stemp = MIFget_string(token, err);
            val.v.vec.sVec = (void *) REALLOC(val.v.vec.sVec,
                                     (val.v.numValue + 1) * sizeof(char *));
            val.v.vec.sVec[val.v.numValue] = stemp;
            val.v.numValue++;
            break;

        case  IF_CPLXVEC:
            ctemp = MIFget_complex(token, token_type, line, err);
            val.v.vec.cVec = (void *) REALLOC(val.v.vec.cVec,
                                     (val.v.numValue + 1) * sizeof(IFcomplex));
            val.v.vec.cVec[val.v.numValue] = ctemp;
            val.v.numValue++;
            break;


        default:
            *err = "Internal error - unexpected value type in MIFgetValue()";
            return(NULL);

        }

        if(*err)
            return(NULL);

        /* exit after this single pass if not array */
        if(! is_array)
            break;

    } /* end forever loop */


    return(&val);
} 


/* *************************************************************** */


static int MIFget_boolean(char *token, char **err)
{
    *err = NULL;

    if((strcmp(token, "t") == 0) || (strcmp(token, "true") == 0))
        return(1);
    else if((strcmp(token, "f") == 0) || (strcmp(token, "false") == 0))
        return(0);
    else
        *err = "Bad boolean value";

    return(-1);
}


/* *************************************************************** */

static int MIFget_integer(char *token, char **err)
{
    int     error;
    long    l;
    double  dtemp;
    char    *endp;
    long    strtol(char *, char **, int);

    *err = NULL;

    l = strtol(token, &endp, 0);  /* handles base 8, 10, 16 automatically */

    /* if error, probably caused by engineering suffixes, */
    /* so try parsing with INPevaluate */
    if(errno || (*endp != '\0')) {
        dtemp = INPevaluate(&token, &error, 1);
        if(error) {
            *err = "Bad integer, octal, or hex value";
            l = 0;
        }
        else if(dtemp > 0.0)
            l = dtemp + 0.5;
        else
            l = dtemp - 0.5;
    }

    return((int) l);
}


/* *************************************************************** */

static double MIFget_real(char *token, char **err)
{
    double  dtemp;
    int     error;

    *err = NULL;

    dtemp = INPevaluate(&token, &error, 1);

    if(error)
        *err = "Bad real value";

    return(dtemp);
}


/* *************************************************************** */

static char *MIFget_string(char *token, char **err)
{
    *err = NULL;

    return(token);
}


/* *************************************************************** */

static IFcomplex MIFget_complex(char *token, Mif_Token_Type_t token_type,
                                char **line, char **err)
{
    static  char    *msg = "Bad complex value";

    IFcomplex  ctemp;

    double  dtemp;
    int     error;

    *err = NULL;

    ctemp.real = 0.0;
    ctemp.imag = 0.0;

    /* Complex values must be of form < <real> <real> > */
    if(token_type != MIF_LCOMPLEX_TOK) {
        *err = msg;
        return(ctemp);
    }

    /* get the real part */
    token = MIFget_token(line, &token_type);
    if(token_type != MIF_STRING_TOK) {
        *err = msg;
        return(ctemp);
    }
    dtemp = INPevaluate(&token, &error, 1);
    if(error) {
        *err = msg;
        return(ctemp);
    }
    ctemp.real = dtemp;

    /* get the imaginary part */
    token = MIFget_token(line, &token_type);
    if(token_type != MIF_STRING_TOK) {
        *err = msg;
        return(ctemp);
    }
    dtemp = INPevaluate(&token, &error, 1);
    if(error) {
        *err = msg;
        return(ctemp);
    }
    ctemp.imag = dtemp;

    /* eat the closing > delimiter */
    token = MIFget_token(line, &token_type);
    if(token_type != MIF_RCOMPLEX_TOK) {
        *err = msg;
        return(ctemp);
    }

    return(ctemp);
}

