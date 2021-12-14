/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /*
     * SMPprint(matrix)
     *      - prints out the matrix in a 'reasonable'
     *      format for debugging
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "suffix.h"

RCSID("SMPprint.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:43 $")

void
SMPprint(matrix,file)
    SMPmatrix * matrix;
    FILE *file;

{
    register int i; 
    register SMPelement *cur;

    (void)fprintf(file,"\n\n");
    (void)fprintf(file,"===================================================\n");
    (void)fprintf(file,"**************  Matrix debugging dump  ************\n");
    (void)fprintf(file,"**************    Matrix information   ************\n");
    (void)fprintf(file,"\n\n");
    (void)fprintf(file,"Matrix is at location %X, and has %d rows and columns\n"
            ,matrix, matrix->SMPsize);
    (void)fprintf(file,"\n\n");
    (void)fprintf(file,
        " row     row      col     num     num     first      first \n");
    (void)fprintf(file,
        "  or    real     real      in      in       in         in  \n");
    (void)fprintf(file,
        " col    name     name     row     col      row        col  \n");
    (void)fprintf(file," num.\n");
    (void)fprintf(file,"\n");
    for (i=0 ; i <= matrix->SMPsize ; i++) {
        (void)fprintf(file,"%4d   %5d    %5d    %4d    %4d   %8X   %8X\n",
            i,
            *(matrix->SMProwMapOut +i ),
            *(matrix->SMPcolMapOut +i ),
            *(matrix->SMProwCount +i ),
            *(matrix->SMPcolCount +i ),
            *(matrix->SMProwHead +i ),
            *(matrix->SMPcolHead +i )  );
    }
    (void)fprintf(file,"\n\n");
    (void)fprintf(file,"===================================================\n");
    (void)fprintf(file,"**************  Matrix debugging dump  ************\n");
    (void)fprintf(file,"**************   Element information   ************\n");
    (void)fprintf(file,"\n\n");
    (void)fprintf(file,
            " elem    intern intern     next       next        value of");
    (void)fprintf(file,"element\n");
    (void)fprintf(file," addr      row    col       in         in\n");
    (void)fprintf(file,
            "          numb   numb      row        col          real   ");
    (void)fprintf(file,"imaginary\n");
    (void)fprintf(file,"\n");
    for (i=0;i<=matrix->SMPsize;i++) {
        cur= *(matrix->SMProwHead +i );
        while(cur != NULL) {
            (void)fprintf(file,"%8X  %5d  %5d   %8X   %8X  %13.6e  %13.6e\n",
                cur,
                cur->SMProwNumber,
                cur->SMPcolNumber,
                cur->SMProwNext,
                cur->SMPcolNext,
                cur->SMPvalue,
                cur->SMPiValue);
            cur = cur->SMProwNext;
        }
    }
}

