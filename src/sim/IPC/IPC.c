/*============================================================================
FILE    IPC.c

MEMBER OF process XSPICE

Copyright 1991
Georgia Tech Research Corporation
Atlanta, Georgia 30332
All Rights Reserved

PROJECT A-8503

AUTHORS

    9/12/91  Steve Tynor

MODIFICATIONS

    6/13/92  Bill Kuhn  Added some comments

SUMMARY

    Provides compatibility for the new SPICE simulator to both the MSPICE user
    interface and BCP (via ATESSE v.1 style AEGIS mailboxes) and the new ATESSE
    v.2 Simulator Interface and BCP (via Bsd Sockets).

    The Interprocess Communications package provides functions
    called to receive XSPICE decks from the ATESSE Simulator Interface
    or Batch Control processes, and to return results to those
    processes.  Functions callable from the simulator packages include:
    
        ipc_initialize_server
        ipc_terminate_server
        ipc_get_line
        ipc_send_line
        ipc_send_data_prefix
        ipc_send_data_suffix
        ipc_send_dcop_prefix
        ipc_send_dcop_suffix
        ipc_send_evtdict_prefix
        ipc_send_evtdict_suffix
        ipc_send_evtdata_prefix
        ipc_send_evtdata_suffix
        ipc_send_errchk
        ipc_send_end
        ipc_send_boolean
        ipc_send_int
        ipc_send_double
        ipc_send_complex
        ipc_send_event
        ipc_flush
    
    These functions communicate with a set of transport-level functions
    that implement the interprocess communications under one of
    the following protocol types determined by a compile-time option:
    
        BSD UNIX Sockets
        HP/Apollo Mailboxes
    
    For each transport protocol, the following functions are written:
    
        ipc_transport_initialize_server
        ipc_transport_get_line
        ipc_transport_terminate_server
        ipc_transport_send_line



============================================================================*/

#ifndef NDEBUG
#include <stdio.h>
#endif
#include <sys/file.h>   /* Specific to BSD - Use sys/fcntl.h for sys5 */
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>     /* NOTE: I think this is a Sys5ism (there is not man
                         * page for it under Bsd, but it's in /usr/include
                         * and it has a BSD copyright header. Go figure.
                         */

#include "IPC.h"
#include "IPCtiein.h"
#include "IPCproto.h"


/*
 * Conditional compilation sanity check:
 */
#if !defined (IPC_AEGIS_MAILBOXES) && !defined (IPC_UNIX_SOCKETS)\
   && !defined (IPC_DEBUG_VIA_STDIO)
"       compiler error - must specify a transport mechanism";
#endif



/*
 * static 'globals'
 */

/*typedef unsigned char Buffer_Char_t;*/
typedef char Buffer_Char_t;

#define OUT_BUFFER_SIZE 1000
#define MAX_NUM_RECORDS 200
static int              end_of_record_index [MAX_NUM_RECORDS];
static int              num_records;
static Buffer_Char_t    out_buffer [OUT_BUFFER_SIZE];
static int              fill_count;

static Ipc_Mode_t       mode;
static Ipc_Protocol_t   protocol;
static Ipc_Boolean_t        end_of_deck_seen;
static int              batch_fd;

#define FMT_BUFFER_SIZE 80
static char fmt_buffer [FMT_BUFFER_SIZE];

/*---------------------------------------------------------------------------*/
static Ipc_Boolean_t kw_match (keyword, str)
     char *keyword;
     char *str;
     /*
      * returns IPC_TRUE if the first `strlen(keyword)' characters of `str' match
      * the ones in `keyword' - case sensitive
      */
{
   char *k = keyword;
   char *s = str;

   /*
    * quit if we run off the end of either string:
    */
   while (*s && *k) {
      if (*s != *k) {
         return IPC_FALSE;
      }
      s++;
      k++;
   }
   /*
    * if we get this far, it sould be because we ran off the end of the 
    * keyword else we didn't match:
    */
   return (*k == '\0');
}

/*---------------------------------------------------------------------------*/

/*
ipc_initialize_server

This function creates the interprocess communication channel
server mailbox or socket.
*/


Ipc_Status_t ipc_initialize_server (server_name, m, p)
     char               *server_name;  /* Mailbox path or host/portnumber pair */
     Ipc_Mode_t         m;             /* Interactive or batch */
     Ipc_Protocol_t     p;             /* Type of IPC protocol */
     /*
      * For mailboxes, `server_name' would be the mailbox pathname; for
      * sockets, this needs to be a host/portnumber pair. Maybe this should be
      * automatically generated by the routine...
      */
{
   Ipc_Status_t status;
   char batch_filename [1025];
   
   mode = m;
   protocol = p;
   end_of_deck_seen = IPC_FALSE;

   num_records = 0;
   fill_count = 0;
   
   status = ipc_transport_initialize_server (server_name, m, p,
                                             batch_filename);

   if (status != IPC_STATUS_OK) {
      fprintf (stderr, "ERROR: IPC: error initializing server\n");
      return IPC_STATUS_ERROR;
   }

   if (mode == IPC_MODE_BATCH) {
#ifdef IPC_AEGIS_MAILBOXES
      strcat (batch_filename, ".log");
#endif
      batch_fd = open (batch_filename, O_WRONLY | O_CREAT, 0666);
      if (batch_fd < 0) {
         fprintf (stderr, "ERROR: IPC: Error opening batch output file: %s\n",
                  batch_filename);
         perror ("IPC");
         return IPC_STATUS_ERROR;
      }
   }
   return status;
}

/*---------------------------------------------------------------------------*/

/*
ipc_terminate_server

This function deallocates the interprocess communication channel
mailbox or socket.
*/


Ipc_Status_t ipc_terminate_server ()
{
   return ipc_transport_terminate_server ();
}

/*---------------------------------------------------------------------------*/

/*
ipc_get_line

This function gets a SPICE deck input line from the interprocess
communication channel.  Any special control commands in the deck
beginning with a ``>'' or ``#'' character are processed internally by
this function and not returned to SPICE.
*/

Ipc_Status_t ipc_get_line (str, len, wait)
     char               *str;   /* Text retrieved from IPC channel */
     int                *len;   /* Length of text string */
     Ipc_Wait_t         wait;   /* Select blocking or non-blocking */
     /*
      * Reads one SPICE line from the connection. Strips any control lines
      * which cannot be interpretted by the simulator (e.g. >INQCON) and
      * processes them.  If such a line is read, it is processed and the next
      * line is read.  `ipc_get_line' does not return until a non-interceptable
      * line is read or end of file.
      *
      * If `wait' is IPC_NO_WAIT and there is no data available on the
      * connection, `ipc_get_line' returns IPC_STATUS_NO_DATA. If `wait' is
      * IPC_WAIT, `ipc_get_line' will not return until there is data available
      * or and end of file condition is reached or an error occurs.
      *
      * Intercepts and processes the following commands:
      *    #RETURNI, #MINTIME, #VTRANS,
      *    >PAUSE, >CONT, >STOP, >INQCON, >NETLIST, >ENDNET
      * Other > records are silently ignored.
      *
      * Intercepts old-style .TEMP card generated by MSPICE
      *
      * Returns:
      *    IPC_STATUS_OK                - for successful reads
      *    IPC_STATUS_NO_DATA           - when NO_WAIT and no data available
      *    IPC_STATUS_END_OF_DECK       - at end of deck (>ENDNET seen)
      *    IPC_STATUS_ERROR             - otherwise
      */
{
   Ipc_Status_t status;
   Ipc_Boolean_t need_another = IPC_TRUE;
   
   do {

      status = ipc_transport_get_line (str, len, wait);
      
      switch (status) {
      case IPC_STATUS_NO_DATA:
      case IPC_STATUS_ERROR:
         need_another = IPC_FALSE;
         break;
      case IPC_STATUS_END_OF_DECK:
         assert (0); /* should never get this from the low-level get-line */
         status = IPC_STATUS_ERROR;
         need_another = IPC_FALSE;
         break;
      case IPC_STATUS_OK:
         /*
          * Got a good line - check to see if it's one of the ones we need to
          * intercept
          */
         if (str[0] == '>') {
            if (kw_match (">STOP", str)) {
               ipc_handle_stop();
            } else if (kw_match (">PAUSE", str)) {
               /* assert (need_another); */
               /*
                * once more around the loop to do a blocking wait for the >CONT
                */
               need_another = IPC_TRUE;
               wait = IPC_WAIT;
            } else if (kw_match (">INQCON", str)) {
               ipc_send_line (">ABRTABL");
               ipc_send_line (">PAUSABL");
               ipc_send_line (">KEEPABL");
               status = ipc_flush ();
               if (IPC_STATUS_OK != status) {
                  need_another = IPC_FALSE;
               }
            } else if (kw_match (">ENDNET", str)) {
               end_of_deck_seen = IPC_TRUE;
               need_another = IPC_FALSE;
               status = IPC_STATUS_END_OF_DECK;
            } else {
               /* silently ignore */
            }
         } else if (str[0] == '#') {
            if (kw_match ("#RETURNI", str)) {
               ipc_handle_returni ();
            } else if (kw_match ("#MINTIME", str)) {
               double d1, d2;
               if (1 != sscanf (&str[8], "%lg", &d1)) {
                  status = IPC_STATUS_ERROR;
                  need_another = IPC_FALSE;
               } else {
                  ipc_handle_mintime (d1);
               }
            } else if (kw_match ("#VTRANS", str)) {
               char *tok1;
               char *tok2;
               char *tok3;
               
               tok1 = &str[8];
               for (tok2 = tok1; *tok2; tok2++) {
                  if (isspace(*tok2)) {
                     *tok2 = '\0';
                     tok2++;
                     break;
                  }
               }
               for(tok3 = tok2; *tok3; tok3++) {
                   if(isspace(*tok3)) {
                       *tok3 = '\0';
                       break;
                   }
               }
               ipc_handle_vtrans (tok1, tok2);
            } else {
               /* silently ignore */
            }
         } else if (str[0] == '.') {
            if (kw_match (".TEMP", str)) {
               /* don't pass .TEMP card to caller */
               printf("Old-style .TEMP card found - ignored\n");
            }
            else {
               /* pass all other . cards to the caller */
               need_another = IPC_FALSE;
            }
         } else {
            /*
             * Not a '>' or '#' record - let the caller deal with it
             */
            need_another = IPC_FALSE;
         }
         break;
      default:
         /*
          * some unknown status value!
          */
         assert (0);
         status = IPC_STATUS_ERROR;
         need_another = IPC_FALSE;
         break;
      }
   } while (need_another);

   return status;
}

/*---------------------------------------------------------------------------*/

/*
ipc_flush

This function flushes the interprocess communication channel
buffer contents.
*/

Ipc_Status_t ipc_flush ()
     /*
      * Flush all buffered messages out the connection.
      */
{
   Ipc_Status_t status;
   int last = 0;
   int bytes;
   int i;

   /* if batch mode */
   if (mode == IPC_MODE_BATCH) {

      assert (batch_fd >= 0);

      /* for number of records in buffer */
      for (i = 0; i < num_records; i++) {

         /* write the records to the .log file */
         if ((end_of_record_index [i] - last) !=
               write (batch_fd, &out_buffer[last], end_of_record_index [i] - last)) {
            fprintf (stderr,
                     "ERROR: IPC: Error writing to batch output file\n");
            perror ("IPC");
            return IPC_STATUS_ERROR;
         }

         /* If the record is one of the batch simulation status messages, */
         /* send it over the ipc channel too */
         if( kw_match("#ERRCHK",  &out_buffer[last]) ||
             kw_match(">ENDANAL", &out_buffer[last]) ||
             kw_match(">ABORTED", &out_buffer[last]) ) {

            status = ipc_transport_send_line (&out_buffer[last],
                                              end_of_record_index [i] - last);
            if (IPC_STATUS_OK != status) {
               return status;
            }
         }
         last = end_of_record_index [i];
      }

   /* else, must be interactive mode */
   } else {
      /* send the full buffer over the ipc channel */
      status = ipc_transport_send_line (&out_buffer[0],
                   end_of_record_index [num_records - 1]);
      if (IPC_STATUS_OK != status) {
         return status;
      }
   }

   /* reset counts to zero and return */
   num_records = 0;
   fill_count = 0;
   return IPC_STATUS_OK;
}

/*---------------------------------------------------------------------------*/
static Ipc_Status_t ipc_send_line_binary (str, len)
     char *str;
     int  len;
     /*
      * Same as `ipc_send_line' except does not expect the str to be null
      * terminated. Sends exactly `len' characters. Use this for binary data
      * strings that may have embedded nulls.
      *
      * Modified by wbk to append newlines for compatibility with
      * ATESSE 1.0
      *
      */
{
   int length = len + 1;
   int diff;
   Ipc_Status_t status;

   /*
    * If we can't add the whole str to the buffer, or if there are no more
    * record indices free, flush the buffer:
    */
   if (((fill_count + length) >= OUT_BUFFER_SIZE) ||
       (num_records >= MAX_NUM_RECORDS)) {
      status = ipc_flush ();
      if (IPC_STATUS_OK != status) {
         return status;
      }
   }
   
   /*
    * make sure that the str will fit:
    */
   if (length + fill_count > OUT_BUFFER_SIZE) {
      fprintf (stderr,
               "ERROR: IPC: String too long to fit in output buffer (> %d bytes) - truncated\n",
               OUT_BUFFER_SIZE);
      length = OUT_BUFFER_SIZE - fill_count;
   }

   /*
    * finally, concatenate the str to the end of the buffer and add the newline:
    */
   memcpy (&out_buffer[fill_count], str, len);
   fill_count += len;

   out_buffer[fill_count] = '\n';
   fill_count++;

   end_of_record_index [num_records++] = fill_count;
   
   return IPC_STATUS_OK;
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_line

This function sends a line of text over the interprocess
communication channel.
*/


Ipc_Status_t ipc_send_line (str)
     char               *str;     /* The text to send */
{ 
   int len;
   int send_len;

   char  *s;

   Ipc_Status_t  status;


   len = strlen(str);

   /* if short string, send it immediately */
   if(len < 80)
      status = ipc_send_line_binary (str, len);
   else {
      /* otherwise, we have to send it as multiple strings */
      /* because Mspice cannot handle things longer than 80 chars */
      s = str;
      while(len > 0) {
         if(len < 80)
            send_len = len;
         else
            send_len = 79;
         status = ipc_send_line_binary (str, send_len);
         if(status != IPC_STATUS_OK)
            break;
         s += send_len;
         len -= send_len;
      }
   }

   return(status);
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_data_prefix

This function sends a ``>DATAB'' line over the interprocess
communication channel to signal that this is the beginning of a
results dump for the current analysis point.
*/

Ipc_Status_t ipc_send_data_prefix (time)
     double             time;    /* The analysis point for this data set */
{
   char buffer[40];

   sprintf (buffer, ">DATAB %.5E", time);
   return ipc_send_line (buffer);
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_data_suffix

This function sends a ``>ENDDATA'' line over the interprocess
communication channel to signal that this is the end of a results
dump from a particular analysis point.
*/


Ipc_Status_t ipc_send_data_suffix ()
{
   Ipc_Status_t  status;

   status = ipc_send_line (">ENDDATA");

   if(status != IPC_STATUS_OK)
       return(status);

   return(ipc_flush());
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_dcop_prefix

This function sends a ``>DCOPB'' line over the interprocess
communication channel to signal that this is the beginning of a
results dump from a DC operating point analysis.
*/

Ipc_Status_t ipc_send_dcop_prefix ()
{
   return ipc_send_line (">DCOPB");
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_dcop_suffix

This function sends a ``>ENDDATA'' line over the interprocess
communication channel to signal that this is the end of a results
dump from a particular analysis point.
*/


Ipc_Status_t ipc_send_dcop_suffix ()
{
   Ipc_Status_t  status;

   status = ipc_send_line (">ENDDCOP");

   if(status != IPC_STATUS_OK)
       return(status);

   return(ipc_flush());
}


/*---------------------------------------------------------------------------*/

/*
ipc_send_evtdict_prefix

This function sends a ``>EVTDICT'' line over the interprocess
communication channel to signal that this is the beginning of an
event-driven node dictionary.

The line is sent only if the IPC is configured
for UNIX sockets, indicating use with the V2 ATESSE SI process.
*/

Ipc_Status_t ipc_send_evtdict_prefix ()
{
#ifdef IPC_AEGIS_MAILBOXES
   return IPC_STATUS_OK;
#else
   return ipc_send_line (">EVTDICT");
#endif
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_evtdict_suffix

This function sends a ``>ENDDICT'' line over the interprocess
communication channel to signal that this is the end of an
event-driven node dictionary.

The line is sent only if the IPC is configured
for UNIX sockets, indicating use with the V2 ATESSE SI process.
*/


Ipc_Status_t ipc_send_evtdict_suffix ()
{
#ifdef IPC_AEGIS_MAILBOXES
   return IPC_STATUS_OK;
#else
   Ipc_Status_t  status;

   status = ipc_send_line (">ENDDICT");

   if(status != IPC_STATUS_OK)
       return(status);

   return(ipc_flush());
#endif
}


/*---------------------------------------------------------------------------*/

/*
ipc_send_evtdata_prefix

This function sends a ``>EVTDATA'' line over the interprocess
communication channel to signal that this is the beginning of an
event-driven node data block.

The line is sent only if the IPC is configured
for UNIX sockets, indicating use with the V2 ATESSE SI process.
*/

Ipc_Status_t ipc_send_evtdata_prefix ()
{
#ifdef IPC_AEGIS_MAILBOXES
   return IPC_STATUS_OK;
#else
   return ipc_send_line (">EVTDATA");
#endif
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_evtdata_suffix

This function sends a ``>ENDDATA'' line over the interprocess
communication channel to signal that this is the end of an
event-driven node data block.

The line is sent only if the IPC is configured
for UNIX sockets, indicating use with the V2 ATESSE SI process.
*/


Ipc_Status_t ipc_send_evtdata_suffix ()
{
#ifdef IPC_AEGIS_MAILBOXES
   return IPC_STATUS_OK;
#else
   Ipc_Status_t  status;

   status = ipc_send_line (">ENDDATA");

   if(status != IPC_STATUS_OK)
       return(status);

   return(ipc_flush());
#endif
}


/*---------------------------------------------------------------------------*/

/*
ipc_send_errchk

This function sends a ``\ERRCHK [GO|NOGO]'' message over the
interprocess communication channel to signal that the initial
parsing of the input deck has been completed and to indicate
whether or not errors were detected.
*/


Ipc_Status_t ipc_send_errchk()
{
    char str[IPC_MAX_LINE_LEN+1];
    Ipc_Status_t  status;

    if(g_ipc.errchk_sent)
        return(IPC_STATUS_OK);

    if(g_ipc.syntax_error)
        sprintf(str, "#ERRCHK NOGO");
    else
        sprintf(str, "#ERRCHK GO");

    g_ipc.errchk_sent = IPC_TRUE;

    status = ipc_send_line(str);
    if(status != IPC_STATUS_OK)
        return(status);

    return(ipc_flush());
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_end

This function sends either an ``>ENDANAL'' or an ``>ABORTED'' message
over the interprocess communication channel together with the
total CPU time used to indicate whether or not the simulation
completed normally.
*/


Ipc_Status_t ipc_send_end()
{
    char str[IPC_MAX_LINE_LEN+1];
    Ipc_Status_t  status;

    if(g_ipc.syntax_error || g_ipc.run_error)
        sprintf(str, ">ABORTED %.4f", g_ipc.cpu_time);
    else
        sprintf(str, ">ENDANAL %.4f", g_ipc.cpu_time);

    status = ipc_send_line(str);
    if(status != IPC_STATUS_OK)
        return(status);

    return(ipc_flush());
}


/*---------------------------------------------------------------------------*/
static int stuff_binary_v1 (d1, d2, n, buf, pos)
     double     d1, d2;         /* doubles to be stuffed                */
     int        n;              /* how many of d1, d2 ( 1 <= n <= 2 )   */
     char       *buf;           /* buffer to stuff to                   */
     int        pos;            /* index at which to stuff              */
{
   union {
      float float_val[2];
      char ch[32];
   } trick;
   int i, j;
   
   assert (protocol == IPC_PROTOCOL_V1);
   assert (sizeof(float) == 4);
   assert (sizeof(char)  == 1);
   assert ((n >= 1) && (n <= 2));

   trick.float_val[0] = d1;
   if (n > 1) {
      trick.float_val[1] = d2;
   }
   for (i = 0, j = pos; i < n*sizeof(float); j++, i++)
      buf[j] = trick.ch[i];
   i = sizeof(float)*n + pos;
   buf[0] = 'A' + i - 1; 
   return i;
}

/*---------------------------------------------------------------------------*/


/*
ipc_send_double

This function sends a double data value over the interprocess
communication channel preceded by a character string that
identifies the simulation variable.
*/

Ipc_Status_t ipc_send_double (tag, value)
     char               *tag;    /* The node or instance */
     double             value;   /* The data value to send */
{
   int i;
   int len;
   int fmt_buffer_len;
           
   switch (protocol) {
   case IPC_PROTOCOL_V1:
      strcpy (fmt_buffer, " "); /* save room for the length byte */
      strcat (fmt_buffer, tag);
      strcat (fmt_buffer, " ");

      /* If talking to Mentor tools, must force upper case for Mspice 7.0 */
      fmt_buffer_len = strlen(fmt_buffer);
      for(i = 0; i < fmt_buffer_len; i++) {
          if(islower(fmt_buffer[i]))
              fmt_buffer[i] = toupper(fmt_buffer[i]);
      }

      len = stuff_binary_v1 (value, 0.0, 1, fmt_buffer, strlen(fmt_buffer));
      break;
   case IPC_PROTOCOL_V2:
      break;
   }
   return ipc_send_line_binary (fmt_buffer, len);
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_complex

This function sends a complex data value over the interprocess
communication channel preceded by a character string that
identifies the simulation variable.
*/


Ipc_Status_t ipc_send_complex (tag, value)
     char               *tag;    /* The node or instance */
     Ipc_Complex_t      value;   /* The data value to send */
{
   int i;
   int len;
   int fmt_buffer_len;
           
   switch (protocol) {
   case IPC_PROTOCOL_V1:
      strcpy (fmt_buffer, " "); /* save room for the length byte */
      strcat (fmt_buffer, tag);
      strcat (fmt_buffer, " ");

      /* If talking to Mentor tools, must force upper case for Mspice 7.0 */
      fmt_buffer_len = strlen(fmt_buffer);
      for(i = 0; i < fmt_buffer_len; i++) {
          if(islower(fmt_buffer[i]))
              fmt_buffer[i] = toupper(fmt_buffer[i]);
      }

      len = stuff_binary_v1 (value.real, value.imag, 2, fmt_buffer,
                             strlen(fmt_buffer));
      break;
   case IPC_PROTOCOL_V2:
      break;
   }
   return ipc_send_line_binary (fmt_buffer, len);
}

/*---------------------------------------------------------------------------*/

/*
ipc_send_event

This function sends data from an event-driven node over the interprocess
communication channel.  The data is sent only if the IPC is configured
for UNIX sockets, indicating use with the V2 ATESSE SI process.
*/


Ipc_Status_t ipc_send_event(ipc_index, step, plot_val, print_val, ipc_val, len)
    int         ipc_index;      /* Index used in EVTDICT */
    double      step;           /* Analysis point or timestep (0.0 for DC) */
    double      plot_val;       /* The value for plotting purposes */
    char        *print_val;     /* The value for printing purposes */
    void        *ipc_val;       /* The binary representation of the node data */
    int         len;            /* The length of the binary representation */
{
#ifdef IPC_AEGIS_MAILBOXES
   return IPC_STATUS_OK;
#else
   char         buff[OUT_BUFFER_SIZE];
   int          i;
   int          buff_len;
   char         *buff_ptr;
   char         *temp_ptr;
   float        fvalue;

   /* Report error if size of data is too big for IPC channel block size */
   if((len + strlen(print_val) + 100) >= OUT_BUFFER_SIZE) {
      printf("ERROR - Size of event-driven data too large for IPC channel\n");
      return IPC_STATUS_ERROR;
   }

   /* Place the index into the buffer with a trailing space */
   sprintf(buff, "%d ", ipc_index);

   assert(sizeof(float) == 4);
   assert(sizeof(int) == 4);

   /* Put the analysis step bytes in */
   buff_len = strlen(buff);
   buff_ptr = buff + buff_len;
   fvalue = step;
   temp_ptr = (char *) &fvalue;
   for(i = 0; i < 4; i++) {
      *buff_ptr = temp_ptr[i];
      buff_ptr++;
      buff_len++;
   }

   /* Put the plot value in */
   fvalue = plot_val;
   temp_ptr = (char *) &fvalue;
   for(i = 0; i < 4; i++) {
      *buff_ptr = temp_ptr[i];
      buff_ptr++;
      buff_len++;
   }

   /* Put the length of the binary representation in */
   temp_ptr = (char *) &len;
   for(i = 0; i < 4; i++) {
      *buff_ptr = temp_ptr[i];
      buff_ptr++;
      buff_len++;
   }

   /* Put the binary representation bytes in last */
   temp_ptr = ipc_val;
   for(i = 0; i < len; i++)
      buff_ptr[i] = temp_ptr[i];
   buff_ptr += len;
   buff_len += len;

   /* Put the print value in */
   strcpy(buff_ptr, print_val);
   buff_ptr += strlen(print_val);
   buff_len += strlen(print_val);

   /* Send the data to the IPC channel */
   return ipc_send_line_binary(buff, buff_len);

#endif
}


