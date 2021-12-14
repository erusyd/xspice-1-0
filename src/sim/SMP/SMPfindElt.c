/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /*
     * SMPfindElt.c (matrix, row, col, flag)
     *      - find an element in the Sparse Matrix Package
     *      structure.  matrix is a SMPmatrix pointer,
     *      row and column are row and column numbers, and
     *      the returned value is a pointer to the element
     *      structure.
     *      Row and column must be integers, which will
     *      be mapped as necessary to ensure a compact numbering
     *      set.
     *      flag determines the action if the element does not
     *      already exist:  0 => return NULL
     *              !0 => create new element
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPfindElt.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:31 $")

SMPelement *
SMPfindElt(matrix, row, col, flag) 
    register SMPmatrix * matrix;
    register int row;
    register int col;
    register int flag;

{
register SMPelement **prev;
register SMPelement *new;

    /*
     * go through the row structure and look for the element
     */

    for ( prev = matrix->SMProwHead + row ;
        (*prev != NULL && (*prev)->SMPcolNumber <= col);
        prev = &((*prev)->SMProwNext)) {
        /* note that prev itself can never be null */
        /* looking through list until we find right column */
        if((*prev)->SMPcolNumber == col) {

            /* found it - return value */

            return (*prev);
        }
    }
    /* stepped past it */
    /* it doesn't exist - check flag and either return NULL or create 
     * new element */
    if (flag == 0) return ((SMPelement *) NULL) ;
    new = (SMPelement *) MALLOC(sizeof(SMPelement));
    if(new == (SMPelement *)NULL) return((SMPelement *)NULL); /* no memory! */
    new->SMPvalue = 0;
    new->SMPiValue = 0;
    new->SMProwNext = *prev;
    new->SMProwNumber = row;
    new->SMPcolNumber = col;
    matrix->SMPnonZero++;
    *prev = new;
    if (row!=col && row != 0 && col != 0) {
        /* remember, counts are OFF-DIAGONAL, so skip if on diagonal */
        (*(matrix->SMProwCount + row )) ++;
        (*(matrix->SMPcolCount + col )) ++;
    }
    for ( prev = matrix->SMPcolHead + col ;
        (*prev != NULL && (*prev)->SMProwNumber <= row);
        prev = &((*prev)->SMPcolNext) ) {
            /* note that prev itself 
             * can never be null */
#ifdef DEBUG
        if((*prev)->SMProwNumber == row) {
            /*
             * oops - shouldn't have found
             * it here - something strange!
             */
            DEBUGMSG("Can't possibly get here!");
            ABORT();
        }
#endif
    }
    /* stepped across it - fill in */
    new->SMPcolNext = *prev;
    *prev = new;
    return (new);
}
