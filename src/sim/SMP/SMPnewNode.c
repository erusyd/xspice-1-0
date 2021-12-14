/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * SMPnewNode(node,matrix)
     *      - add a new row and column to the matrix
     *      corresponding to the given node.  Expand all
     *      matrix data structures as necessary
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPnewNode.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:40 $")

#define CHECK(pointer) if(!(pointer)) {return(E_NOMEM);}
#define CKALLOC(pointer,number,type) if(!(matrix->pointer = \
    (type *)REALLOC(matrix->pointer,(number)*sizeof(type)))){ return(E_NOMEM);}

int
SMPnewNode(node,matrix)
    register int node;
    register SMPmatrix *matrix;
{

register int i;
    if (node > matrix->SMPallocSize) {
        matrix->SMPallocSize = node + SMPALLOCINCREMENT;
        CKALLOC(SMProwMapIn,matrix->SMPallocSize+1,int);
        CKALLOC(SMPcolMapIn,matrix->SMPallocSize+1,int);
        CKALLOC(SMProwMapOut,matrix->SMPallocSize+1,int);
        CKALLOC(SMPcolMapOut,matrix->SMPallocSize+1,int);
        CKALLOC(SMProwCount,matrix->SMPallocSize+1,int);
        CKALLOC(SMPcolCount,matrix->SMPallocSize+1,int);
        CKALLOC(SMProwHead,matrix->SMPallocSize+1,SMPelement *);
        CKALLOC(SMPcolHead,matrix->SMPallocSize+1,SMPelement *);

    }
    for (i=matrix->SMPsize+1;i<=node;i++) {
        matrix->SMPsize += 1;
        *(matrix->SMProwCount + matrix->SMPsize) = -1;
        *(matrix->SMPcolCount + matrix->SMPsize) = -1;
        *(matrix->SMProwHead + matrix->SMPsize) = NULL;
        *(matrix->SMPcolHead + matrix->SMPsize) = NULL;
        *(matrix->SMProwMapIn + matrix->SMPsize) = i;
        *(matrix->SMPcolMapIn + matrix->SMPsize) = i;
        *(matrix->SMProwMapOut + matrix->SMPsize) = i;
        *(matrix->SMPcolMapOut + matrix->SMPsize) = i;
    }
    return(OK);
}
