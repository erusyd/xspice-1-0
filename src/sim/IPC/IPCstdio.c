/*
 * Steve Tynor
 *
 * Generic Interprocess Communication module
 *
 * Used for debugging in absense of IPC interface.
 *
 */

#ifdef IPC_DEBUG_VIA_STDIO

#include <stdio.h>

#include "IPC.h"

#include "IPCproto.h"

/*---------------------------------------------------------------------------*/
Ipc_Status_t ipc_transport_initialize_server (server_name, m, p,
                                              batch_filename)
     char               *server_name;
     Ipc_Mode_t         m;
     Ipc_Protocol_t     p;
     char               *batch_filename;
{
   assert (m == IPC_MODE_INTERACTIVE);
   printf ("INITIALIZE_SERVER\n");
   return IPC_STATUS_OK;
}

/*---------------------------------------------------------------------------*/
Ipc_Status_t ipc_transport_get_line (str, len, wait)
     char               *str;
     int                *len;
     Ipc_Wait_t         wait;
{
   printf ("GET_LINE\n");
   gets (str);
   *len = strlen (str);
   return IPC_STATUS_OK;
}
 
/*---------------------------------------------------------------------------*/
Ipc_Status_t ipc_transport_send_line (str, len)
     char *str;
     int len;
{
   int i;

   printf ("SEND_LINE: /");
   for (i = 0; i < len; i++)
      putchar (str[i]);
   printf ("/\n");
   return IPC_STATUS_OK;
}

/*---------------------------------------------------------------------------*/
Ipc_Status_t ipc_transport_terminate_server ()
{
return IPC_STATUS_OK;
}


#endif  /* IPC_DEBUG_VIA_STDIO */
