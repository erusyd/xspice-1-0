/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

/* 
 *  provide the error message appropriate for the given error code
 */

#include "prefix.h"
#include <stdio.h>
#include <string.h>
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "util.h"
#include "suffix.h"

RCSID("INPerror.c $Revision: 1.1 $ on $Date: 91/04/02 11:56:42 $")

#define MSG(a) \
val = MALLOC(2+strlen(a));\
if(val) {\
    (void)strcpy(val,a);\
    break;\
}\
OUTerror(E_PANIC,nomem,(IFuid *)NULL);\
break;

char *
INPerror(type)
    int type;
{

static char *from =         "%sdetected in routine %s\n";
static char *unknownError = "Unknown error code %d\n";
static char *Pause =        "Pause requested\n";
static char *panic =        "Impossible error - can't occur\n";
static char *exists =       "Device already exists, existing one being used\n";
static char *nodev =        "No such device\n";
static char *nomod =        "No such model\n";
static char *noanal =       "No such analysis type\n";
static char *noterm =       "No such terminal on this device\n";
static char *badparm =      "No such parameter on this device\n";
static char *nomem =        "Out of Memory\n";
static char *nodecon =      "Warning: old connection replaced\n";
static char *unsupp =       "Action unsupported by this simulator\n";
static char *parmval =      "Parameter value is illegal\n";
static char *notempty =     "Can't delete - still referenced\n";
static char *nochange =     "Sorry, simulator can't handle that now\n";
static char *notfound =     "Not found\n";

char *val;
char *tmp;

    switch(type) {
        default:
            if(errMsg) {
                val = MALLOC(strlen(errMsg)+5);
                (void) sprintf(val,"%s\n",errMsg);
                FREE(errMsg);
                errMsg = NULL;
                break;
            } else {
                val = MALLOC(strlen(unknownError) +10);
                if(val) {
                    (void) sprintf(val,unknownError,type);
                    break;
                }
            }
            OUTerror(E_PANIC,nomem,(IFuid *)NULL);
            break;
        case E_PAUSE:
            MSG(Pause)
        case OK:
            return(NULL);
        case E_PANIC:
            MSG(panic)
        case E_EXISTS:
            MSG(exists)
        case E_NODEV:
            MSG(nodev)
        case E_NOMOD:
            MSG(nomod)
        case E_NOANAL:
            MSG(noanal)
        case E_NOTERM:
            MSG(noterm)
        case E_BADPARM:
            MSG(badparm)
        case E_NOMEM:
            MSG(nomem)
        case E_NODECON:
            MSG(nodecon)
        case E_UNSUPP:
            MSG(unsupp)
        case E_PARMVAL:
            MSG(parmval)
        case E_NOTEMPTY:
            MSG(notempty)
        case E_NOCHANGE:
            MSG(nochange)
        case E_NOTFOUND:
            MSG(notfound)
    }
    if(errRtn) {
        tmp = MALLOC(strlen(from)+strlen(errRtn)+5);
        sprintf(tmp, from, val ,errRtn);
        FREE(val);
        return(tmp);
    } else {
        return(val);
    }
    /*NOTREACHED*/
}
    
