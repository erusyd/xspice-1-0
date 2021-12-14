
/*
 * Copyright (c) 1988 Jeffrey M. Hsu
 *
 * Stand-alone input routine.
 */

#include "prefix.h"
#include "FTEinput.h"
#include "suffix.h"

extern int inchar();

void
Input(request, response)
REQUEST *request;
RESPONSE *response;
{

    switch (request->option) {
      case char_option:
        response->reply.ch = inchar(request->fp);
        response->option = request->option;
        break;
      default:
        /* just ignore, since we don't want a million error messages */
        response->option = error_option;
        break;
    }
    return;

}
