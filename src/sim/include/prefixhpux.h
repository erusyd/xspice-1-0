
/* Local parameters, specific to HP-UX on an HP Bobcat (tommy.berkeley.edu) */

/* Basic system definition parameters. */

#define UNIX        /* Compiling under unix (signals, processes, shell) */
#define HPUX        /* HP-UX */

/* Graphics packages available. */

#define GI_X
#define GI_MFB

/* Pathnames, hostnames, and addresses. */

#define BUGADDR "spice@ic.berkeley.edu"    /* Where to send bugs. */
#define HELPPATH "~faustus/spice3lib/helpdir"
#define LIBPATH "~faustus/spice3lib/scripts"
#define NEWSFILE "~faustus/spice3lib/news"    /* The message of the day. */
#define SPICEDLOG "/eros1/faustus/spiced/logfile"
#define SPICEHOST "eros"                /* Where to do rspice, "" = nowhere */
#define SPICEPATH "/users/faustus/bin/spice3"

/* Assorted configuration parameters. */

#define FTEDEBUG    /* Enable general debugging */
#define CPDEBUG     /* Enable cshpar debugging */
#define DEF_EDITOR "vi"
#define ASCIIRAWFILE true   /* true or false - default file type = ascii */
#define NOLIST
#define NOPRINT
#define NOBYPASS
#define NEWCONV

/* Machine dependent capabilities. */

#define MAXPOSINT 2147483647
#define MAX_EXP_ARG 87  /* Approximate largest arg to exp() without error. */
#define ASCIIQUOTE  /* Use 8th-bit quoting for characters */

/* Functions available in the C library. */

#define HAS_QSORT
#define HAS_VSPRINTF

/* And things not in this implementation. */

#define index strchr
#define rindex strrchr
#define void int

#ifdef LINT
#define RCSID(ident)
#else /* LINT */
#define RCSID(ident) static char rcsid[] = ident ;
#endif /* LINT */
