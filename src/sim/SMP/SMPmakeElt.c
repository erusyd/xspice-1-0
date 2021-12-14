/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /*
     * SMPmakeElt.c (matrix, row, col)
     *      - add an element to the Sparse Matrix Package
     *      structure.  matrix is a SMPmatrix pointer,
     *      row and column are row and column numbers, and
     *      the returned value is a pointer to the 'value'
     *      field.  
     *      Row and column must be integers, which will
     *      be mapped as necessary to ensure a compact numbering
     *      set.
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "suffix.h"

RCSID("SMPmakeElt.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:36 $")

double *
SMPmakeElt(matrix, row, col) 
    register SMPmatrix * matrix;
    register int row;
    register int col;

{
register SMPelement *new;

    if(row > (matrix->SMPsize)) {
        if(SMPnewNode(row,matrix)) {
            return((double *)NULL); /* no memory! */
        }
    }

    if(col > (matrix->SMPsize)) {
        if(SMPnewNode(col,matrix)) {
            return((double *)NULL); /* no memory! */
        }
    }

    row = SMPextToIntMapRow(row,matrix);
    col = SMPextToIntMapCol(col,matrix);

    new = SMPfindElt(matrix,row,col,1);
    if(new == (SMPelement *)NULL) {
        return((double *)NULL); /* no memory! */
    }
    return (&(new->SMPvalue));
    
}
