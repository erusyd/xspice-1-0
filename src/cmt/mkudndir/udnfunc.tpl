
/* Do not delete the following include file entry */

#include "EVTudn.h"



void *malloc(unsigned);



/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_create(CREATE_ARGS)
{
}


/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_dismantle(DISMANTLE_ARGS)
{
}


/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_initialize(INITIALIZE_ARGS)
{
}


/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_invert(INVERT_ARGS)
{
}


/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_resolve(RESOLVE_ARGS)
{
}

/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_copy(COPY_ARGS)
{
}


/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_compare(COMPARE_ARGS)
{
}


/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_plot_val(PLOT_VAL_ARGS)
{
}


/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_print_val(PRINT_VAL_ARGS)
{
}



/* ---------------------------------------------------------- */

void udn_NODE_TYPE_NAME_ipc_val(IPC_VAL_ARGS)
{
}


/* ---------------------------------------------------------- */


Evt_Udn_Info_t udn_NODE_TYPE_NAME_info = {

    "NODE_TYPE_NAME",
    "Put description of node type   NODE_TYPE_NAME   here!",

    udn_NODE_TYPE_NAME_create,
    udn_NODE_TYPE_NAME_dismantle,
    udn_NODE_TYPE_NAME_initialize,
    udn_NODE_TYPE_NAME_invert,
    udn_NODE_TYPE_NAME_copy,
    udn_NODE_TYPE_NAME_resolve,
    udn_NODE_TYPE_NAME_compare,
    udn_NODE_TYPE_NAME_plot_val,
    udn_NODE_TYPE_NAME_print_val,
    udn_NODE_TYPE_NAME_ipc_val

};
