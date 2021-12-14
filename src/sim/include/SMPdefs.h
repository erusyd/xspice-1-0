/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef SMP
#define SMP "SMPdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:38 $ "

#include "complex.h"
#include <stdio.h>

    /*
     * SMPdefs.h - structures for sparse matrix package
     */

    typedef struct sSMPelement {
            /* next two entries (value,iValue) MUST remain adjacent and in */
            /* that order due to the definition of 'complex' used */
        double SMPvalue;    /* matrix entry for this point */
        double SMPiValue;   /* imaginary matrix entry for this point */
        struct sSMPelement *SMProwNext;  /* pointer to next in row */
        struct sSMPelement *SMPcolNext;  /* pointer to next in column */
        int SMProwNumber; /* internal row number */
        int SMPcolNumber; /* internal column number */
    } SMPelement;


    typedef struct {
            /* all below except size are actually arrays
             * but the compiler insists they be declared
             * as pointers since I can't specify how big
             * they are at compile time - they are all
             * allocated dynamically.
             */
        SMPelement **SMProwHead;    /* pointer to first in row */
        SMPelement **SMPcolHead;    /* pointer to first in col */
        int *SMProwCount;   /* number in row excluding diag -1 */
        int *SMPcolCount;   /* number in column excluding diag -1 */
        int *SMProwMapIn;   /* mapping array from external to 
                             * internal row numbers */
        int *SMPcolMapIn;   /* mapping array from external to
                             * internal column numbers */
        int *SMProwMapOut;  /* mapping array from internal to 
                             * external row numbers */
        int *SMPcolMapOut;  /* mapping array from internal to
                             * external column numbers */
        int SMPsize;        /* number of rows and columns */
        int SMPallocSize;   /* number of rows and columns alocated */
        int SMPnonZero;     /* number of non-zero terms in matrix */
        int SMPoldNonZ;     /* number of non-zero terms in
                             * matrix before reordering */
        int SMPbadi;        /* row of last troublesome entry */
        int SMPbadj;        /* column of last troublesome entry*/
    }SMPmatrix;

    typedef struct {
        int SMPorder;
        int SMPnonZeros;
        int SMPnonZerosBeforeReorder;
        double SMPlargestDiag;
        double SMPsmallestDiag;
        double SMPlargestNonZero;
        double SMPsmallestNonZero;
    } SMPstatistics;

#define SMPextToIntMapRow(row,matrix) (*((matrix->SMProwMapIn)+row))
#define SMPextToIntMapCol(col,matrix) (*((matrix->SMPcolMapIn)+col))
#define SMPintToExtMapRow(row,matrix) (*((matrix->SMProwMapOut)+row))
#define SMPintToExtMapCol(col,matrix) (*((matrix->SMPcolMapOut)+col))
#define GROUNDNODE(matrix) 0

/* amount we grow the SMP tables by as the matrix grows */
#define SMPALLOCINCREMENT 100

#ifdef __STDC__
int SMPaddElt( SMPmatrix *, int , int , double );
void SMPcClear( SMPmatrix *);
int SMPcLUfac( SMPmatrix *, double );
int SMPcProdDiag( SMPmatrix *, SPcomplex *, int *);
int SMPcReorder( SMPmatrix * , double , double , int *);
void SMPcSolve( SMPmatrix *, double [], double [], double [], double []);
void SMPclear( SMPmatrix *);
void SMPcolSwap( SMPmatrix * , int , int );
void SMPdestroy( SMPmatrix *);
int SMPfillup( SMPmatrix * );
SMPelement * SMPfindElt( SMPmatrix *, int , int , int );
void SMPgetError( SMPmatrix *, int *, int *);
int SMPluFac( SMPmatrix *, double , double );
double * SMPmakeElt( SMPmatrix * , int , int );
int SMPmatSize( SMPmatrix *);
int SMPnewMatrix( SMPmatrix ** );
int SMPnewNode( int , SMPmatrix *);
int SMPpreOrder( SMPmatrix *);
void SMPprint( SMPmatrix * , FILE *);
int SMPreorder( SMPmatrix * , double , double , double );
void SMProwSwap( SMPmatrix * , int , int );
void SMPsolve( SMPmatrix *, double [], double []);
#else /* stdc */
int SMPaddElt();
void SMPcClear();
int SMPcLUfac();
int SMPcProdDiag();
int SMPcReorder();
void SMPcSolve();
void SMPclear();
void SMPcolSwap();
void SMPdestroy();
int SMPfillup();
SMPelement * SMPfindElt();
void SMPgetError();
int SMPluFac();
double * SMPmakeElt();
int SMPmatSize();
int SMPnewMatrix();
int SMPnewNode();
int SMPpreOrder();
void SMPprint();
int SMPreorder();
void SMProwSwap();
void SMPsolve();
#endif /* stdc */

#endif /*SMP*/
