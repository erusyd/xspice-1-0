/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SMPmatSize(matrix)
     *  returns the size of the matrix
     */

#include "prefix.h"
#include "SMPdefs.h"
#include "suffix.h"

RCSID("SMPmatSize.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:38 $")

int
SMPmatSize(matrix)
    register SMPmatrix *matrix;
{
    return(matrix->SMPsize);
}
