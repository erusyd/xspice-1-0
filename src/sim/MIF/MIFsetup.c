/*============================================================================
FILE    MIFsetup.c

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

    This file contains the function called by SPICE to setup data structures
    of a code model after parsing, but prior to beginning a simulation.  The
    major responsibilities of this function are to default values for model
    parameters not given on the .model card, create equations in the matrix
    for any voltage sources, and setup the matrix pointers used during
    simulation to load the matrix.

INTERFACES

    MIFsetup()

REFERENCED FILES

    None.

NON-STANDARD FEATURES

    None.

============================================================================*/


#include "prefix.h"
#include <stdio.h>
#include "util.h"
#include "SMPdefs.h"
#include "DEVdefs.h"
#include "SPerror.h"

#include "MIFproto.h"
#include "MIFparse.h"
#include "MIFdefs.h"
#include "MIFcmdat.h"

#include "suffix.h"



extern SPICEdev *DEVices[];        /* info about all device types */



/* define macro for easy creation of matrix entries/pointers for outputs */
#define TSTALLOC(ptr,first,second) \
        if((smp_data_out->ptr = \
            SMPmakeElt(matrix, smp_data_out->first, smp_data_out->second)) == NULL) { \
            return(E_NOMEM); \
        }

/* define macro for easy creation of matrix entries/pointers for inputs */
#define CTSTALLOC(ptr,first,second) \
        if((smp_data_out->input[k].port[l].ptr = \
            SMPmakeElt(matrix, smp_data_out->first, smp_data_cntl->second)) == NULL) { \
            return(E_NOMEM); \
        }



/*
MIFsetup

This function is called by the CKTsetup() driver function to
prepare all code model structures and all code model instance
structures for simulation.  It loops through all models of a
particular code model type and provides defaults for any
parameters not specified on a .model card.  It loops through all
instances of the model and prepares the instance structures for
simulation.  The most important setup task is the creation of
entries in the SPICE matrix and the storage of pointers to
locations of the matrix used by MIFload during a simulation.
*/


int
MIFsetup(
    SMPmatrix     *matrix,    /* The analog simulation matrix structure */
    GENmodel      *inModel,   /* The head of the model list */
    CKTcircuit    *ckt,       /* The circuit structure */
    int           *states)    /* The states vector */
{
    MIFmodel    *model;
    MIFinstance *here;

    int         mod_type;
    int         max_size;
    int         size;
    int         error;

    int         num_conn;
    int         num_port;
    int         num_port_k;
    int         i;
    int         j;
    int         k;
    int         l;

    Mif_Port_Type_t  type;
    Mif_Port_Type_t  in_type;
    Mif_Port_Type_t  out_type;

    Mif_Cntl_Src_Type_t  cntl_src_type;

    Mif_Smp_Ptr_t  *smp_data_out;
    Mif_Smp_Ptr_t  *smp_data_cntl;

    Mif_Param_Info_t  *param_info;
    Mif_Conn_Info_t   *conn_info;

    Mif_Boolean_t   is_input;
    Mif_Boolean_t   is_output;

    char        *suffix;
    CKTnode     *tmp;


    /* Setup for access into MIF specific model data */
    model = (MIFmodel *) inModel;
    mod_type = model->MIFmodType;


    /* loop through all models of this type */

    for( ; model != NULL; model = model->MIFnextModel) {


        /* For each parameter not given explicitly on the .model */
        /* card, default it                                      */

        for(i = 0; i < model->num_param; i++) {

            if(model->param[i]->is_null) {

                /* setup a pointer for quick access */
                param_info = &(DEVices[mod_type]->DEVpublic.param[i]);

                /* determine the size and allocate the parameter element(s) */
                if(! param_info->is_array) {
                    model->param[i]->size = 1;
                    model->param[i]->element = (void *) MALLOC(sizeof(Mif_Value_t));
                }
                else {  /* parameter is an array */
                    /* MIF_INP2A() parser assures that there is an associated array connection */
                    /* Since several instances may share this model, we have to create an array    */
                    /* big enough for the instance with the biggest connection array               */
                    max_size = 0;
                    for(here = model->MIFinstances; here != NULL; here = here->MIFnextInstance) {
                        size = here->conn[param_info->conn_ref]->size;
                        if(size > max_size)
                            max_size = size;
                    }
                    model->param[i]->size = max_size;
                    model->param[i]->element = (void *) MALLOC(max_size * sizeof(Mif_Value_t));
                }  /* end if parameter is an array */

                /* set the parameter element(s) to default value */
                for(j = 0; j < model->param[i]->size; j++) {

                    switch(param_info->type) {

                    case MIF_BOOLEAN:
                        model->param[i]->element[j].bvalue = param_info->default_value.bvalue;
                        break;

                    case MIF_INTEGER:
                        model->param[i]->element[j].ivalue = param_info->default_value.ivalue;
                        break;

                    case MIF_REAL:
                        model->param[i]->element[j].rvalue = param_info->default_value.rvalue;
                        break;

                    case MIF_COMPLEX:
                        model->param[i]->element[j].cvalue = param_info->default_value.cvalue;
                        break;

                    case MIF_STRING:
                        model->param[i]->element[j].svalue = param_info->default_value.svalue;
                        break;

                    default:
                        return(E_BADPARM);
                    }

                } /* end for number of elements in param array */

            }  /* end if null */

        }  /* end for number of parameters */


        /* For each instance, initialize stuff used by cm_... functions */

        for(here = model->MIFinstances; here != NULL; here = here->MIFnextInstance) {

            here->num_state = 0;
            here->state = NULL;

            here->num_intgr = 0;
            here->intgr = NULL;

            here->num_conv = 0;
            here->conv = NULL;
        }


        /* For each instance, allocate runtime structs for output connections/ports */
        /* and grab a place in the state vector for all input connections/ports */

        for(here = model->MIFinstances; here != NULL; here = here->MIFnextInstance) {

            /* Skip these expensive allocations if the instance is not analog */
            if(! here->analog)
                continue;

            num_conn = here->num_conn;
            for(i = 0; i < num_conn; i++) {
                if((here->conn[i]->is_null) || (! here->conn[i]->is_output) )
                    continue;
                num_port = here->conn[i]->size;
                for(j = 0; j < num_port; j++) {
                    here->conn[i]->port[j]->partial =
                                   (void *) MALLOC(num_conn * sizeof(Mif_Partial_t));
                    here->conn[i]->port[j]->ac_gain =
                                   (void *) MALLOC(num_conn * sizeof(Mif_AC_Gain_t));
                    here->conn[i]->port[j]->smp_data.input =
                                   (void *) MALLOC(num_conn * sizeof(Mif_Conn_Ptr_t));
                    for(k = 0; k < num_conn; k++) {
                        if((here->conn[k]->is_null) || (! here->conn[k]->is_input) )
                            continue;
                        num_port_k = here->conn[k]->size;
                        here->conn[i]->port[j]->partial[k].port =
                                   (void *) MALLOC(num_port_k * sizeof(double));
                        here->conn[i]->port[j]->ac_gain[k].port =
                                   (void *) MALLOC(num_port_k * sizeof(Mif_Complex_t));
                        here->conn[i]->port[j]->smp_data.input[k].port =
                                   (void *) MALLOC(num_port_k * sizeof(Mif_Port_Ptr_t));
                    }
                }
            }

            num_conn = here->num_conn;
            for(i = 0; i < num_conn; i++) {
                if((here->conn[i]->is_null) || (! here->conn[i]->is_input) )
                    continue;
                num_port = here->conn[i]->size;
                for(j = 0; j < num_port; j++) {
                    here->conn[i]->port[j]->old_input = *states;
                    (*states)++;
                }
            }
        }


        /* Loop through all instances of this model and for each port of each connection */
        /* create current equations, matrix entries, and matrix pointers as necessary.   */

        for(here = model->MIFinstances; here != NULL; here = here->MIFnextInstance) {

            /* Skip these expensive allocations if the instance is not analog */
            if(! here->analog)
                continue;

            num_conn = here->num_conn;

            /* loop through all connections on this instance */
            /* and create matrix data needed for outputs and */
            /* V sources associated with I inputs            */
            for(i = 0; i < num_conn; i++) {

                /* if the connection is null, skip to next connection */
                if(here->conn[i]->is_null)
                    continue;

                /* prepare things for convenient access later */
                is_input = here->conn[i]->is_input;
                is_output = here->conn[i]->is_output;
                num_port = here->conn[i]->size;

                /* loop through all ports on this connection */
                for(j = 0; j < num_port; j++) {

                    /* if port is null, skip to next */
                    if(here->conn[i]->port[j]->is_null)
                        continue;

                    /* determine the type of this port */
                    type = here->conn[i]->port[j]->type;

                    /* create a pointer to the smp data for quick access */
                    smp_data_out = &(here->conn[i]->port[j]->smp_data);

                    /* if it has a voltage source output, */
                    /* create the matrix data needed      */
                    if( (is_output && (type == MIF_VOLTAGE || type == MIF_DIFF_VOLTAGE)) ||
                                     (type == MIF_RESISTANCE || type == MIF_DIFF_RESISTANCE) ) {

                        /* first, make the current equation */
                        suffix = (void *) MALLOC(strlen((char *) here->MIFname) + 100);
                        sprintf(suffix, "branch_%d_%d", i, j);
                        error = CKTmkCur(ckt, &tmp, here->MIFname, suffix);
                        FREE(suffix);
                        if(error)
                            return(error);
                        smp_data_out->branch = tmp->number;

                        /* ibranch is needed to find the input equation for RESISTANCE type */
                        smp_data_out->ibranch = tmp->number;

                        /* then make the matrix pointers */
                        TSTALLOC(pos_branch, pos_node, branch);
                        TSTALLOC(neg_branch, neg_node, branch);
                        TSTALLOC(branch_pos, branch,  pos_node);
                        TSTALLOC(branch_neg, branch,  neg_node);
                    } /* end if current input */

                    /* if it is a current input */
                    /* create the matrix data needed for the associated zero-valued V source */
                    if(is_input && (type == MIF_CURRENT || type == MIF_DIFF_CURRENT)) {

                        /* first, make the current equation */
                        suffix = (void *) MALLOC(strlen((char *) here->MIFname) + 100);
                        sprintf(suffix, "ibranch_%d_%d", i, j);
                        error = CKTmkCur(ckt, &tmp, here->MIFname, suffix);
                        FREE(suffix);
                        if(error)
                            return(error);
                        smp_data_out->ibranch = tmp->number;

                        /* then make the matrix pointers */
                        TSTALLOC(pos_ibranch, pos_node, ibranch);
                        TSTALLOC(neg_ibranch, neg_node, ibranch);
                        TSTALLOC(ibranch_pos, ibranch,  pos_node);
                        TSTALLOC(ibranch_neg, ibranch,  neg_node);
                    } /* end if current input */

                    /* if it is a vsource current input (refers to a vsource elsewhere */
                    /* in the circuit), locate the source and get its equation number  */
                    if(is_input && (type == MIF_VSOURCE_CURRENT)) {
                        smp_data_out->ibranch = CKTfndBranch(ckt,
                                                 here->conn[i]->port[j]->vsource_str);
                        if(smp_data_out->ibranch == NULL) {
                            IFuid names[2];
                            names[0] = here->MIFname;
                            names[1] = (IFuid) here->conn[i]->port[j]->vsource_str;
                            (*(SPfrontEnd->IFerror))(ERR_FATAL,
                                    "%s: unknown controlling source %s",names);
                            return(E_BADPARM);
                        }
                    } /* end if vsource current input */

                } /* end for number of ports */
            } /* end for number of connections */

            /* now loop through all connections on the instance and create */
            /* matrix data needed for partial derivatives of outputs       */
            for(i = 0; i < num_conn; i++) {

                /* if the connection is null or is not an output */
                /* skip to next connection */
                if((here->conn[i]->is_null) || (! here->conn[i]->is_output))
                    continue;

                /* loop through all ports on this connection */

                num_port = here->conn[i]->size;
                for(j = 0; j < num_port; j++) {

                    /* if port is null, skip to next */
                    if(here->conn[i]->port[j]->is_null)
                        continue;

                    /* determine the type of this output port */
                    out_type = here->conn[i]->port[j]->type;

                    /* create a pointer to the smp data for quick access */
                    smp_data_out = &(here->conn[i]->port[j]->smp_data);

                    /* for this port, loop through all connections */
                    /* and all ports to touch on each possible input */
                    for(k = 0; k < num_conn; k++) {

                        /* if the connection is null or is not an input */
                        /* skip to next connection */
                        if((here->conn[k]->is_null) || (! here->conn[k]->is_input))
                            continue;

                        num_port_k = here->conn[k]->size;
                        /* loop through all the ports of this connection */
                        for(l = 0; l < num_port_k; l++) {

                            /* if port is null, skip to next */
                            if(here->conn[k]->port[l]->is_null)
                                continue;

                            /* determine the type of this input port */
                            in_type = here->conn[k]->port[l]->type;

                            /* create a pointer to the smp data for quick access */
                            smp_data_cntl = &(here->conn[k]->port[l]->smp_data);

                            /* determine type of controlled source according */
                            /* to input and output types */
                            cntl_src_type = MIFget_cntl_src_type(in_type, out_type);

                            switch(cntl_src_type) {
                            case MIF_VCVS:
                                CTSTALLOC(e.branch_poscntl, branch, pos_node);
                                CTSTALLOC(e.branch_negcntl, branch, neg_node);
                                break;
                            case MIF_ICIS:
                                CTSTALLOC(f.pos_ibranchcntl, pos_node, ibranch);
                                CTSTALLOC(f.neg_ibranchcntl, neg_node, ibranch);
                                break;
                            case MIF_VCIS:
                                CTSTALLOC(g.pos_poscntl, pos_node, pos_node);
                                CTSTALLOC(g.pos_negcntl, pos_node, neg_node);
                                CTSTALLOC(g.neg_poscntl, neg_node, pos_node);
                                CTSTALLOC(g.neg_negcntl, neg_node, neg_node);
                                break;
                            case MIF_ICVS:
                                CTSTALLOC(h.branch_ibranchcntl, branch, ibranch);
                                break;
                            } /* end switch on controlled source type */
                        } /* end for number of input ports */
                    } /* end for number of input connections */
                } /* end for number of output ports */
            } /* end for number of output connections */

        } /* end for all instances */


    }  /* end for all models of this type */

    return(OK);
}


