/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /*
     * SMPaddElt.c (matrix, row, col, value)
     *      - add an element to the Sparse Matrix Package
     *      structure.  matrix is a SMPmatrix pointer,
     *      row and column are row and column numbers, and
     *      value is the value to be added to the given
     *      location.  value must be of type double
     *      and row and column must be integers, which will
     *      be mapped as necessary to ensure a compact numbering
     *      set.
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPaddElt.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:16 $")

int
SMPaddElt(matrix, row, col, value) 
    SMPmatrix * matrix;
    int row;
    int col;
    double value;

{
    register double *temp;

    temp = (SMPmakeElt(matrix,row,col)) ;
    if(temp) {
        *temp += value;
        return(0);
    }
    return(E_NOMEM);
}
