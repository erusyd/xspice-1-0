
/*
    $Header: //pepper/atesse_spice/spice3/include/RCS/FTEredirect.h,v 1.1 91/04/02 11:25:58 bill Exp $
    Copyright (c) 1987 Jeffrey M. Hsu

    This include files intercepts call that may output to the screen.
*/

#ifdef MENU
#ifndef DIRECT

#define fprintf myfprintf
#define printf  myprintf
#define fflush  myfflush
#ifdef putc
#undef putc
#endif
#define putc    myputc
#define fputs   myfputs

#endif
#endif
