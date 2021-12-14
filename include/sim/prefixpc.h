
/* Local parameters, specific to PC & MS-DOS, Lattice CC (vogon.berkeley.edu) */

/* Basic system definition parameters. */

#define IBMPC
/* we usually use this compiler, so */
/*#define LATTICE /* defined by their newer compiler versions, so not needed */

/* Graphics packages available. */

#define GI_MFB      /* We have the MFB graphics package. */

/* Pathnames, hostnames, and addresses. */

#define SPICEPATH "bspice"   /* Where the spice-3 binary is kept */
#define LIBPATH "\\\\spice3c1\\\\lib\\\\scripts"
#define HELPPATH "\\spice3c1\\lib\\helpdir"

/* Assorted configuration parameters. */

#define ASCIIRAWFILE true
#define DEF_EDITOR "vi"
#define FTEDEBUG    /* Enable general debugging */
#define CPDEBUG     /* Enable cshpar debugging */
#define SHORTMACRO  /* compiler only allows short macros, so use functions */
#define MINDATA /* minimize static dataspace as much as possible */
#define NOINFABS    /* no inline FABS expansion - use functional form */
#define NOPRINT
#define NOLIST
#define NOBYPASS
#define NOSENS /* eliminate sensitivity code due to lack of space */
#define NEWCONV

/* Machine dependent capabilities. */

#define ASCIIQUOTE  /* Use 8th-bit quoting for characters */
#define MAXPOSINT 32767 /* largest possible positive integer (PC=16 bit) */
#define MAX_EXP_ARG 87  /* Approximate largest arg to exp() without error. */

/* Functions available in the C library. */

#define HAS_QSORT
#define HAS_PERROR  /* has own definition of perror in stdio, not ansi's void */
#define NOVARARGS

/* And omissions. */

typedef long time_t;

#ifdef LATTICE
#define mktemp(a) tmpnam(a)
#endif /* LATTICE */

#ifdef LINT
#define RCSID(ident)
#else /* LINT */
/* we'd really like the following line, but memory restrictions 
 * really limit use here, so....
 #define RCSID(ident) static char rcsid[] = ident ;
 */
#define RCSID(ident)
#endif /* LINT */
