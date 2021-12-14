/*============================================================================
FILE    real/udnfunc.c

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

    This file contains the definition of the 'real' node type
    used by event-driven models that simulate with real type data.
    These functions are called exclusively through function
    pointers in an Evt_Udn_Info_t data structure.

INTERFACES

    Evt_Udn_Info_t udn_real_info

REFERENCED FILES

    None.

NON-STANDARD FEATURES

    None.

============================================================================*/

#include <stdio.h>
#include "CM.h"

#include "EVTudn.h"

void *malloc(unsigned);



/* ************************************************************************ */

void udn_real_create(CREATE_ARGS)
{
    /* Malloc space for a real struct */
    MALLOCED_PTR = malloc(sizeof(double));
}


/* ************************************************************************ */

void udn_real_dismantle(DISMANTLE_ARGS)
{
    /* Do nothing.  There are no internally malloc'ed things to dismantle */
}


/* ************************************************************************ */

void udn_real_initialize(INITIALIZE_ARGS)
{
    double  *real_struct = STRUCT_PTR;


    /* Initialize to zero */
    *real_struct = 0.0;
}


/* ************************************************************************ */

void udn_real_invert(INVERT_ARGS)
{
    double      *real_struct = STRUCT_PTR;


    /* Invert the state */
    *real_struct = -(*real_struct);
}


/* ************************************************************************ */

void udn_real_resolve(RESOLVE_ARGS)
{
    double **array    = INPUT_STRUCT_PTR_ARRAY;
    double *out       = OUTPUT_STRUCT_PTR;
    int    num_struct = INPUT_STRUCT_PTR_ARRAY_SIZE;

    double      sum;
    int         i;

    /* Sum the values */
    for(i = 0, sum = 0.0; i < num_struct; i++)
        sum += *(array[i]);

    /* Assign the result */
    *out = sum;
}

/* ************************************************************************ */

void udn_real_copy(COPY_ARGS)
{
    double  *real_from_struct = INPUT_STRUCT_PTR;
    double  *real_to_struct   = OUTPUT_STRUCT_PTR;

    /* Copy the structure */
    *real_to_struct = *real_from_struct;
}


/* ************************************************************************ */

void udn_real_compare(COMPARE_ARGS)
{
    double  *real_struct1 = STRUCT_PTR_1;
    double  *real_struct2 = STRUCT_PTR_2;

    /* Compare the structures */
    if((*real_struct1) == (*real_struct2))
        EQUAL = TRUE;
    else
        EQUAL = FALSE;
}


/* ************************************************************************ */

void udn_real_plot_val(PLOT_VAL_ARGS)
{
    double   *real_struct = STRUCT_PTR;


    /* Output a value for the real struct */
    PLOT_VAL = *real_struct;
}


/* ************************************************************************ */

void udn_real_print_val(PRINT_VAL_ARGS)
{
    double   *real_struct = STRUCT_PTR;


    /* Allocate space for the printed value */
    PRINT_VAL = malloc(30);

    /* Print the value into the string */
    sprintf(PRINT_VAL, "%15.6e", *real_struct);
}



/* ************************************************************************ */

void udn_real_ipc_val(IPC_VAL_ARGS)
{
    /* Simply return the structure and its size */
    IPC_VAL = STRUCT_PTR;
    IPC_VAL_SIZE = sizeof(double);
}



Evt_Udn_Info_t udn_real_info = {

    "real",
    "real valued data",

    udn_real_create,
    udn_real_dismantle,
    udn_real_initialize,
    udn_real_invert,
    udn_real_copy,
    udn_real_resolve,
    udn_real_compare,
    udn_real_plot_val,
    udn_real_print_val,
    udn_real_ipc_val

};

