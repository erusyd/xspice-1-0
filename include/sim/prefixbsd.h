
/* Local parameters, specific to 4.3 BSD on a vax (ic.berkeley.edu) */

/* Basic system definition parameters. */

#define UNIX        /* Compiling under UNIX (signals, processes, shell) */
#define BSD     /* Compiling under BSD UNIX (command completion) */
#define VAX     /* Use vax floating point error codes (with BSD) */

/* Graphics packages available. */

#define GI_X        /* We have the X window system. GI_X11 for V11R3*/
#define GI_MFB      /* We have the MFB graphics package. */

/* Pathnames, hostnames, and addresses. */

#define BUGADDR "spice@ic.berkeley.edu"    /* Where to send bugs. */
#define HELPPATH "~cad/lib/spice3/helpdir"
#define LIBPATH "~cad/lib/spice3/scripts"
#define NEWSFILE "~cad/lib/spice3/news"    /* The message of the day. */
#define DEFAULTMFBCAP "~cad/lib/spice3/mfbcap"    /* The mfbcap file. */
#define SPICEDLOG "/eros1/faustus/spiced/logfile"
#define SPICEHOST "eros"        /* Where to do rspice, "" = nowhere */
#define SPICEPATH "/cad/bin/spice3" /* Where the spice-3 binary is kept */

/* Assorted configuration parameters. */

#define ASCIIRAWFILE true   /* true or false - default file type = ascii */
#define DEF_EDITOR "vi"
#define FTEDEBUG    /* Enable general debugging */
#define CPDEBUG     /* Enable cshpar debugging */
#define CAPZEROBYPASS
#define NEWCONV

/* Machine dependent capabilities. */

#define MAXPOSINT 2147483647
#define MAX_EXP_ARG 87.0 /* Approximate largest arg to exp() without error. */
#define ASCIIQUOTE  /* Use 8th-bit quoting for characters */

/* Functions available in the C library. */

#define HAS_QSORT
#define HAS_BSTRING
#define HAS_INDEX
#define HAS_TIOCSTI
/* #define HAS_VSPRINTF */

#ifdef LINT
#define RCSID(ident)
#else /* LINT */
#define RCSID(ident) static char rcsid[] = ident ;
#endif /* LINT */

