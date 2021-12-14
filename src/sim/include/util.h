/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef UTIL
#define UTIL "util.h $Revision: 1.1 $  on $Date: 91/04/02 11:27:10 $ "

#define MALLOC(x) calloc(1,(unsigned)(x))
#define FREE(x) {free((char *)(x));(x) = 0;}
extern char *malloc();
extern char *calloc();
extern void free();
#define TRUE 1
#define FALSE 0
#define REALLOC(x,y) realloc((char *)(x),(unsigned)(y))
char *realloc();
#ifdef DEBUG
#define DEBUGMSG(textargs) printf(textargs)
#else
#define DEBUGMSG(testargs) 
#endif
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#ifdef NOINFABS
#define FABS(a) fabs(a)
double fabs();
#else /* NOINFABS */
#define FABS(a) ( ((a)<0) ? -(a) : (a) )
#endif /*NOINFABS*/

#define SIGN(a,b) ( b >= 0 ? (a >= 0 ? a : - a) : (a >= 0 ? - a : a))
#define ABORT() fflush(stderr);fflush(stdout);abort();
#ifndef PI
#define PI 3.14159265358979323846
#endif /*PI*/

#endif /*UTIL*/
