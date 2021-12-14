
/* Local parameters, specific to 4.3 BSD on a vax (ic.berkeley.edu) */

/* gtri - add - wbk - 10/8/90 - add comment */

/* ***************************************************** */
/*  Modified to run on Apollo OS 10.2 under BSD 4.3 X11  */
/* ***************************************************** */

/* gtri - end - wbk - 10/8/90 */


/* Basic system definition parameters. */

#define UNIX        /* Compiling under UNIX (signals, processes, shell) */
#define BSD     /* Compiling under BSD UNIX (command completion) */

/* gtri - modify - wbk - 10/8/90 - comment out */
/* #define VAX */    /* Use vax floating point error codes (with BSD) */
/* gtri - end - wbk - 10/8/90 */

/* Graphics packages available. */

/* gtri - modify - wbk - 10/8/90 - change to GI_X11 */
#define GI_X11        /* We have the X window system. GI_X11 for V11R3*/
/* gtri - end - wbk - 10/8/90 */

#define GI_MFB      /* We have the MFB graphics package. */

/* Pathnames, hostnames, and addresses. */

/* gtri - modify - wbk - 10/8/90 - change paths */
#define BUGADDR "xspice@prism.gatech.edu"    /* Where to send bugs. */
#define HELPPATH "/usr/local/xspice-1-0/lib/sim/helpdir"
#define LIBPATH "/usr/local/xspice-1-0/lib/sim/scripts"
#define NEWSFILE "/usr/local/xspice-1-0/lib/sim/news"    /* The message of the day. */
#define DEFAULTMFBCAP "/usr/local/xspice-1-0/lib/sim/mfbcap"    /* The mfbcap file. */
#define SPICEDLOG "/tmp/logfile"
#define SPICEHOST ""        /* Where to do rspice, "" = nowhere */
#define SPICEPATH "/usr/local/xspice-1-0/bin" /* Where the binary is kept */
/* gtri - end - wbk - 10/8/90 */

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

/* gtri - modify - wbk - 10/8/90 - uncomment, we have VSPRINTF */
#define HAS_VSPRINTF
/* gtri - end - wbk - 10/8/90 */

#ifdef LINT
#define RCSID(ident)
#else /* LINT */
#define RCSID(ident) static char rcsid[] = ident ;
#endif /* LINT */

