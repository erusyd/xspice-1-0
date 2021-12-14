/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SMPcolSwap(matrix,col1,col2)
     *      - swap col1 and col2 in the given matrix.
     *      this includes correcting all references,
     *      links, and numbering fields to swap them
     *      as far as the numerics are concerned, but
     *      to still be able to get to them correctly
     *      from their external mapping.
     *      (col1 and col2 are internal col numbers)
     */
    
#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "suffix.h"

RCSID("SMPcolSwap.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:26 $")

void
SMPcolSwap(matrix,col1,col2)
    register SMPmatrix * matrix;
    register int col1;
    register int col2;
{

register SMPelement * cola;
register SMPelement * colb;
SMPelement * rowtemp;
register SMPelement ** rowa;
SMPelement ** rowb;
int choice;
int whichRow;
int a;
int b;

    /* first, correct the master tables for mapping in and out
     * so we can forget about that stuff , also do the col counts
     * and other per-col stuff in the matrix descriptor
     */

    a = *(matrix->SMPcolMapOut + col1) ;
    b = *(matrix->SMPcolMapOut + col2) ;
    *(matrix->SMPcolMapOut + col1) = b;
    *(matrix->SMPcolMapOut + col2) = a;
    *(matrix->SMPcolMapIn + a) = col2;
    *(matrix->SMPcolMapIn + b) = col1;
    a= *(matrix->SMPcolCount + col1) ;
    *(matrix->SMPcolCount + col1) = *(matrix->SMPcolCount + col2) ;
    *(matrix->SMPcolCount + col2) = a;


    /* Now, swap the col pointers for the two cols 
     */

    cola = *(matrix->SMPcolHead + col1) ;
    colb = *(matrix->SMPcolHead + col2) ;
    *(matrix->SMPcolHead + col1) = colb;
    *(matrix->SMPcolHead + col2) = cola;


    /* Now, walk across the two cols in parallel.  Always work on
     * whichever col has the lowest numbered unprocessed row.
     * drop across that row from the top to reach the col (lower
     * numbered col if both cols have data in that row),
     * so that we can grab the element directly above the two cols
     * to swap the cols properly.
     * (note special choice - if moving from col n to col n+k, and
     * across pointer from col n goes beyond col n+k, no action needed
     * except to correct col number in element)
     */

    /* cola is pointer to first in col with lowest old col number */
    if (col1>col2) {
        a = col1;
        col1 = col2;
        col2 = a;
        cola = *(matrix->SMPcolHead + col2) ;
        colb = *(matrix->SMPcolHead + col1) ;
    } /* else already set above during col pointer swap */

    /* go until run off the end of both cols */
    while ((cola != NULL) | (colb != NULL )) {
        /* find out which row to work on */
        if (cola == NULL) {
            whichRow = colb->SMProwNumber;
            choice = 2;
        } else if (colb == NULL) {
            whichRow = cola->SMProwNumber;
            choice = 1;
        } else if (cola->SMProwNumber < colb->SMProwNumber) {
            whichRow = cola->SMProwNumber;
            choice = 1;
        } else if (cola->SMProwNumber > colb->SMProwNumber) {
            whichRow = colb->SMProwNumber;
            choice = 2;
        } else if (cola->SMProwNumber == colb->SMProwNumber) {
            whichRow = colb->SMProwNumber;
            choice = 3;
        }
    
        /* run across row indicated until reach entry above col 1 */
        for (rowa = matrix->SMProwHead + whichRow   ;
            *rowa != NULL && (*rowa)->SMPcolNumber < col1;
            rowa = &((*rowa)->SMProwNext)) {
                ; /* just passin thru....... */
        }
#ifdef DEBUG
        if (*rowa == NULL) {
            DEBUGMSG("Can't POSSIBLY get here - data corrupted!");
            ABORT();
        }
#endif /*DEBUG*/
        /* now use new variable and continue across row until 
         * reach entry above second col */
        rowb = rowa;
        for ( ; 
            *rowb != NULL && (*rowb)->SMPcolNumber < col2 ;
            rowb = &((*rowb)->SMProwNext) ) {
                ; /* nothing to do.... */
        }
        switch (choice) {

        case 1:
            /* just need to move entry from col1 to col2 - easy */
    
            /* correct col number */
            cola->SMPcolNumber = col2; 
    
            /* now correct the pointers to move element */
    
            if (rowa != rowb && cola->SMProwNext != *rowb) {
                *rowa = (cola->SMProwNext);
                cola->SMProwNext = *rowb;
                *rowb = cola;
            }

            cola = cola->SMPcolNext;
            break;

        case 2:
            /* just need to move entry from col2 to col1 - easy */
    
            /*correct col number */
            colb->SMPcolNumber = col1; 
    
            /* now correct the pointers to move element */
            if (rowa != rowb) {
                *rowb = (colb->SMProwNext);
                colb->SMProwNext = *rowa;
                *rowa = colb;
            }

            colb = colb->SMPcolNext;
            break;
        case 3:
            /* correct col numbers */
            cola->SMPcolNumber = col2; 
            colb->SMPcolNumber = col1; 

            /* need to swap the two entries in the same row */
            *rowb = cola;
            rowtemp = colb->SMProwNext;
            colb->SMProwNext = cola->SMProwNext;
            cola->SMProwNext = rowtemp;
            *rowa = colb;


            cola = cola->SMPcolNext;
            colb = colb->SMPcolNext;
            break;
        }
    }
}
