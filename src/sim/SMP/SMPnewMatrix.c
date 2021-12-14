/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * SMPnewMatrix(matrix)
     *      - create a new matrix to be managed by the
     *      sparse matrix package.  The 'matrix' is to
     *      be a pointer to the matrix to allow for further
     *      manipulation by the package.
     */

#include "prefix.h"
#include "SPerror.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "suffix.h"

RCSID("SMPnewMatrix.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:39 $")

#define CHECK(pointer) if(!(pointer)) {SMPdestroy(*matrix);return(E_NOMEM);}

int
SMPnewMatrix(matrix)
    register SMPmatrix ** matrix;
{
    /* allocate a matrix, clear all elements to 0, 
        and return */

    (*matrix) = (SMPmatrix *) MALLOC(sizeof(SMPmatrix));
    if(*matrix == (SMPmatrix *)NULL) /* no memory */ return(E_NOMEM); 
    (*matrix)->SMPsize = 0;
    (*matrix)->SMPallocSize = 0;
    CHECK((*matrix)->SMProwCount = (int *)MALLOC(sizeof(int)));
    CHECK((*matrix)->SMPcolCount = (int *)MALLOC(sizeof(int)));
    CHECK((*matrix)->SMProwMapIn = (int *)MALLOC(sizeof(int)));
    CHECK((*matrix)->SMPcolMapIn = (int *)MALLOC(sizeof(int)));
    CHECK((*matrix)->SMProwMapOut = (int *)MALLOC(sizeof(int)));
    CHECK((*matrix)->SMPcolMapOut = (int *)MALLOC(sizeof(int)));
    CHECK((*matrix)->SMProwHead = (SMPelement **)MALLOC(sizeof(SMPelement *)));
    CHECK((*matrix)->SMPcolHead = (SMPelement **)MALLOC(sizeof(SMPelement *)));
    (*matrix)->SMPnonZero = 0;
    (*matrix)->SMPoldNonZ = 0;

    *((*matrix)->SMProwHead) = NULL;
    *((*matrix)->SMPcolHead) = NULL;
    *((*matrix)->SMProwMapIn) = 0;
    *((*matrix)->SMPcolMapIn) = 0;
    *((*matrix)->SMProwMapOut) = 0;
    *((*matrix)->SMPcolMapOut) = 0;
    *((*matrix)->SMProwCount) = 0;
    *((*matrix)->SMPcolCount) = 0;
    return(OK);
}
