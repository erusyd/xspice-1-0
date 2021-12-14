/* Local parameters, specific to CMS on IBM 3090 (cmsa.berkeley.edu) */

/* Basic system definition parameters. */

#define CMS

/* Graphics packages available. */

#define GI_MFB      /* We have the MFB graphics package. */

/* Pathnames, hostnames, and addresses. */
/* Could be seriously outdated, but who cares, CMS has no directories. */

#define BUGADDR "spice@ic.berkeley.edu"    /* Where to send bugs. */
#define HELPPATH "~cad/lib/spice3/helpdir"
#define LIBPATH "~cad/lib/spice3/scripts"
#define NEWSFILE "~cad/lib/spice3/news"    /* The message of the day. */
#define DEFAULTMFBCAP "~cad/lib/spice3/mfbcap"    /* The mfbcap file. */
#define SPICEDLOG "/eros1/faustus/spiced/logfile"
#define SPICEHOST "eros"        /* Where to do rspice, "" = nowhere */
#define SPICEPATH "bspice3" /* Where the spice-3 binary is kept */

/* Assorted configuration parameters. */

#define ASCIIRAWFILE true   /* true or false - default file type = ascii */
#define DEF_EDITOR "vi"
#define FTEDEBUG    /* Enable general debugging */
#define CPDEBUG     /* Enable cshpar debugging */
#define CAPZEROBYPASS
#define SHORTMACRO  /* compiler only allows short macros, so use functions */
#define MINDATA /* minimize static dataspace as much as possible */
#define NOINFABS    /* no inline FABS expansion - use functional form */
#define NOPRINT
#define NOLIST
#define NOBYPASS
#define NOSENS /* eliminate sensitivity code */
#define NEWCONV

/* Machine dependent capabilities. */

#define HUGE    1e31
#define MAXPOSINT 2147483647
#define MAX_EXP_ARG 87.0 /* Approximate largest arg to exp() without error. */

/* Functions available in the C library. */

/* #define HAS_QSORT /* not with the ibm compiler */
/* #define HAS_BSTRING /* not with the ibm compiler */
#define HAS_INDEX
/* #define HASATRIGH /* not with the ibm compiler */
#define HAS_PERROR  /* has own declaration of perror in stdio, NOT ansi void */
#define NOVARARGS

/* And omissions. */

typedef long time_t;

/* FTE fixes */

/* for inp.c */
#define SEPC ' '

/* hack on Jeff's gr_iplot controversy: only on the cms version */
#define gr_iplot gr_plot
#define gr_iplot_end gr_plot_end

#ifdef LINT
#define RCSID(ident)
#else /* LINT */
/* we'd really like the following line, but memory restrictions 
 * really limit use here, so....
 #define RCSID(ident) static char rcsid[] = ident ;
 */
#define RCSID(ident)
#endif /* LINT */
