/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SMProwSwap(matrix,row1,row2)
     *      - swap row1 and row2 in the given matrix.
     *      this includes correcting all references,
     *      links, and numbering fields to swap them
     *      as far as the numerics are concerned, but
     *      to still be able to get to them correctly
     *      from their external mapping.
     *      (row1 and row2 are internal row numbers)
     */
    
#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "suffix.h"

RCSID("SMProwSwap.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:47 $")

void
SMProwSwap(matrix,row1,row2)
    register SMPmatrix * matrix;
    register int row1;
    register int row2;
{

    register SMPelement * rowa;
    register SMPelement * rowb;
    SMPelement * columntemp;
    register SMPelement ** columna;
    SMPelement ** columnb;
    int choice;
    int whichColumn;
    int a;
    int b;

    /* first, correct the master tables for mapping in and out
     * so we can forget about that stuff , also do the row counts
     * and other per-row stuff in the matrix descriptor
     */

    a = *(matrix->SMProwMapOut + row1) ;
    b = *(matrix->SMProwMapOut + row2) ;
    *(matrix->SMProwMapOut + row1) = b;
    *(matrix->SMProwMapOut + row2) = a;
    *(matrix->SMProwMapIn + a ) = row2;
    *(matrix->SMProwMapIn + b ) = row1;
    a = *(matrix->SMProwCount + row1) ;
    *(matrix->SMProwCount + row1) = *(matrix->SMProwCount + row2);
    *(matrix->SMProwCount + row2) = a;


    /* Now, swap the row pointers for the two rows 
     */

    rowa = *(matrix->SMProwHead + row1);
    rowb = *(matrix->SMProwHead + row2);
    *(matrix->SMProwHead + row1) = rowb;
    *(matrix->SMProwHead + row2) = rowa;


    /* Now, walk down the two rows in parallel.  Always work on
     * whichever row has the lowest numbered unprocessed column.
     * drop down that column from the top to reach the row (lower
     * numbered row if both rows have data in that column),
     * so that we can grab the element directly above the two rows
     * to swap the rows properly.
     * (note special choice - if moving from row n to row n+k, and
     * down pointer from row n goes beyond row n+k, no action needed
     * except to correct row number in element)
     */

    /* rowa is pointer to first in row with lowest old row number */
    if (row1>row2) {
        a = row1;
        row1 = row2;
        row2 = a;
        rowa = *(matrix->SMProwHead + row2);
        rowb = *(matrix->SMProwHead + row1);
    } /* else already set above during row pointer swap */

    /* go until run off the end of both rows */
    while ((rowa != NULL) | (rowb != NULL )) {
        /* find out which column to work on */
        if (rowa == NULL) {
            whichColumn = rowb->SMPcolNumber;
            choice = 2;
        } else if (rowb == NULL) {
            whichColumn = rowa->SMPcolNumber;
            choice = 1;
        } else if (rowa->SMPcolNumber < rowb->SMPcolNumber) {
            whichColumn = rowa->SMPcolNumber;
            choice = 1;
        } else if (rowa->SMPcolNumber > rowb->SMPcolNumber) {
            whichColumn = rowb->SMPcolNumber;
            choice = 2;
        } else if (rowa->SMPcolNumber == rowb->SMPcolNumber) {
            whichColumn = rowb->SMPcolNumber;
            choice = 3;
        }
    
        /* run down column indicated until reach entry above row 1 */
        for (columna = matrix->SMPcolHead + whichColumn ;
            *columna != NULL && (*columna)->SMProwNumber < row1;
            columna = &((*columna)->SMPcolNext)) {
                ; /* just passin thru....... */
        }
#ifdef DEBUG
        if (*columna == NULL) {
            DEBUGMSG("Can't POSSIBLY get here - data corrupted!");
            ABORT();
        }
#endif /*DEBUG*/
        /* now use new variable and continue down column until 
         * reach entry above second row */
        columnb = columna;
        for ( ; 
            *columnb != NULL && (*columnb)->SMProwNumber < row2 ;
            columnb = &((*columnb)->SMPcolNext) ) {
                ; /* nothing to do.... */
        }
        switch (choice) {

        case 1:
            /* just need to move entry from row1 to row2 - easy */
    
            /* correct row number */
            rowa->SMProwNumber = row2; 
    
            /* now correct the pointers to move element */
    
            if(columna != columnb && rowa->SMPcolNext != *columnb) {
                *columna = (rowa->SMPcolNext);
                rowa->SMPcolNext = *columnb;
                *columnb = rowa;
            }

            rowa = rowa->SMProwNext;
            break;

        case 2:
            /* just need to move entry from row2 to row1 - easy */
    
            /*correct row number */
            rowb->SMProwNumber = row1; 
    
            /* now correct the pointers to move element */
            if (columna != columnb) {
                *columnb = (rowb->SMPcolNext);
                rowb->SMPcolNext = *columna;
                *columna = rowb;
            }

            rowb = rowb->SMProwNext;
            break;
        case 3:
            /* correct row numbers */
            rowa->SMProwNumber = row2; 
            rowb->SMProwNumber = row1; 

            /* need to swap the two entries in the same column */
            *columnb = rowa;
            columntemp = rowb->SMPcolNext;
            rowb->SMPcolNext = rowa->SMPcolNext;
            rowa->SMPcolNext = columntemp;
            *columna = rowb;

            rowa = rowa->SMProwNext;
            rowb = rowb->SMProwNext;
            break;
        }
    }
}
