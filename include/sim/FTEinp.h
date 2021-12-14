
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 11:25:54 $
 *         $Source: //pepper/atesse_spice/spice3/include/RCS/FTEinp.h,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Note that this definition must be the same as struct card in INPdefs.h...
 */

#ifndef FTEinput_h
#define FTEinput_h

struct line {
    int li_linenum;
    char *li_line;
    char *li_error;
    struct line *li_next;
    struct line *li_actual;
} ;

/* Listing types. */

#define LS_LOGICAL  1
#define LS_PHYSICAL 2
#define LS_DECK  3

#endif /* FTEinput_h */
