
/* Local parameters, specific to V4 VMS on a vax (icw.berkeley.edu) */

/* Basic system definition parameters. */

#define VMS     /* Compiling under VMS */

/* Graphics packages available. */

#define GI_MFB      /* We have the MFB graphics package. */

/* Pathnames, hostnames, and addresses. */

#define SPICEPATH "spice.exe"
#define SPICEHOST ""        /* Where to do rspice, "" = nowhere */
#define NEWSFILE "nutnews.txt"  /* The message of the day. */
#define BUGADDR "spice@ic.berkeley.edu"
#define LIBPATH "DUA1:[FAUSTUS.SPICE3.LIB.SCRIPTS]"
#define HELPPATH "DUA1:[FAUSTUS.SPICE3.LIB.HELPDIR]"

/* Assorted configuration parameters. */ 

#define FTEDEBUG    /* Enable general debugging */
#define CPDEBUG     /* Enable cshpar debugging */
#define DEF_EDITOR "vi"
#define ASCIIRAWFILE true
#define NOLIST
#define NEWCONV


#define MAXPOSINT 2147483647
#define MAX_EXP_ARG 87  /* Approximate largest arg to exp() without error. */
#define ASCIIQUOTE  /* Use 8th-bit quoting for characters */

/* Functions available in the C library. */

/* And omissions. */

/* This should be unnecessary, but VMS leaves out this directive. */
/*#define defined(a) (a) */
/* fixed in vax C V2.3 & later  - uncomment above if using older VAX C */

#define index strchr
#define rindex strrchr

#ifdef LINT
#define RCSID(ident)
#else /* LINT */
#define RCSID(ident) static char rcsid[] = ident ;
#endif /* LINT */
