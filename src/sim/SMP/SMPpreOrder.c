/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SMPpreOrder(matrix)
     *
     *  special subroutine which takes advantage of the knowledge that
     *  we are working with a MNA matrix, and can perform some special
     *  row swaps to make the matrix better-conditioned
     *
     */
    /*
     *
     *  We are looking for the special structure we know we will
     *  find in the equations corresponding to the voltage sources.
     *
     *  What we expect to see is the stamp from the source, which looks
     *  like:
     *
     *      +   -   I
     *  +           1
     *  -          -1
     *  I   1  -1   0
     *
     *  Note that the zero at (I,I) is on the diagonal and MUST be
     *  a zero, since this is the current equation for the source
     *  so no other entries can be made in this equation.
     *  We also note that the other entries are symmetricly placed,
     *  and have magnitude 1.  This means that we can get a BIG win by
     *  swapping, say, row + and row I - this would put 1's on the diagonal
     *  at both (+,+) and (I,I)!  This would be simple, but if rows have
     *  been swapped previously by this routine, one of the rows + or -
     *  may be missing.  If we don't do things carefully, they may
     *  BOTH be gone, and then we're really up a creek.
     *  So - we move along carefully.
     *
     */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "SMPdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPpreOrder.c $Revision: 1.4 $ on $Date: 12/.0/.1 .0:.5:.0 $")

#define TODO 0x1
#define LOCK 0x2

/* our own local subroutine to do grunt work */
#ifdef __STDC__
static int SMPcount( int , SMPmatrix *, char *);
#else /* stdc */
static int SMPcount();
#endif /* stdc */

int
SMPpreOrder(matrix)
    register SMPmatrix *matrix;

{
    register int i;
    register SMPelement *here;
    register SMPelement *there;
    register char *stat;
    register SMPelement *diag;
    register int count;
    register int *work;
    register int jobs=0;
    register int j;
    static char *badmsg ="Improperly formed matrix - shouldn't happen!(preOrd)";

/* gtri - modify - wbk - 4/30/91 - add new variable for bug fix */
    register int jobs_max;
/* gtri - end - wbk - 4/30/91 - add new variable for bug fix */


    stat = (char *)MALLOC((matrix->SMPsize+1)*sizeof(char));
    work = (int *)MALLOC((matrix->SMPsize+1)*sizeof(int));

/* gtri - modify - wbk - 4/30/91 - bug fix */

    /* Line 167 in this routine was found to be writing into space     */
    /* not owned by the 'work' array.                                  */
    /* The problem is that the variable 'jobs' was being incremented   */
    /* to a size greater than the malloc'ed size.                      */
    /* See also four modifications in for loops below */

    /* This variable MUST equal the size allocated for 'work' above */
    jobs_max = matrix->SMPsize+1;

/* gtri - end - wbk - 4/30/91 - bug fix */

    for(i=1;i<= matrix->SMPsize;i++) {
        diag = SMPfindElt(matrix,i,i,0);
        if(diag==(SMPelement *)NULL) {
            stat[i]=TODO;   /* a diagonal zero, so this row needs help */
            /* now to find out how bad off this row is - does it have */
            /* 1 or 2 matched off-diagonal 1's */
            count=SMPcount(i,matrix,stat);
            /* Now if count is 1, then this device has no */
            /* choice of pivot, so we flag it for immediate movement */
            /* otherwise, we'll pick it up later */
            if(count == 1) {
                work[jobs++]=i;
            }
        }
    }

    do {
        while(jobs) {
            i = work[--jobs];
            if (stat[i] != TODO) {
                /* something happened - this row got fixed by an earlier 
                 * iteration so we don't have anything left to do - that 
                 * was easy!
                 */
                continue;
            } 
                
            /* Special information:  this must be a voltage source
             *  current equation or a node connected to nothing but
             *  voltage sources, since that is the only thing that can
             *  put a zero on the diagonal.  
             */
            for(here = *(matrix->SMProwHead + i) ; 
                    here != NULL ; here = here->SMProwNext) {
                if( (here->SMProwNumber != here->SMPcolNumber) &&
                        (here->SMPcolNumber != 0) &&
                        (here->SMPvalue) != 0 ){
                    /* ok - have a candidate - now check credentials */
                    j = here->SMPcolNumber;
                    if(stat[j] != LOCK) {
                        there=SMPfindElt(matrix,j,i,0);
                        if( (there != NULL) && (there->SMPvalue != 0)) {
                            /* ok - passes all the tests I can think of */
                            /* soooo - lock it, and check for consequences */
                            stat[i]=stat[j]=LOCK;
                            /* consequences are same as earlier check on count,
                             * so we do the count operation on all rows/columns
                             * that i or j provided an entry for since we are 
                             * taking them out of the running
                             */
                            for(there = *(matrix->SMProwHead+i);
                                    there != NULL; there = there->SMProwNext) {
                                if(stat[there->SMPcolNumber] == TODO &&
                                        SMPcount(there->SMPcolNumber,matrix,
                                        stat) ==1) {
                                    /* gtri - modify - wbk - 4/30/91 - bug fix */
                                    if(jobs == jobs_max) {
                                        jobs_max *= 2;
                                        work = (int *) REALLOC(work, jobs_max * sizeof(int));
                                        printf("\nSMPpreOrder - 1 allocation of array -work- increased\n");
                                    }
                                    /* gtri - end - wbk - 4/30/91 - bug fix */
                                    work[jobs++]=there->SMPcolNumber;
                                }
                            }
                            for(there = *(matrix->SMProwHead+j);
                                    there != NULL; there = there->SMProwNext) {
                                if(stat[there->SMPcolNumber] == TODO &&
                                        SMPcount(there->SMPcolNumber,matrix,
                                        stat)==1) {
                                    /* gtri - modify - wbk - 4/30/91 - bug fix */
                                    if(jobs == jobs_max) {
                                        jobs_max *= 2;
                                        work = (int *) REALLOC(work, jobs_max * sizeof(int));
                                        printf("\nSMPpreOrder - 2 allocation of array -work- increased\n");
                                    }
                                    /* gtri - end - wbk - 4/30/91 - bug fix */
                                    work[jobs++]=there->SMPcolNumber;
                                }
                            }
                            for(there = *(matrix->SMPcolHead+i);
                                    there != NULL; there = there->SMPcolNext) {
                                if(stat[there->SMProwNumber] == TODO &&
                                        SMPcount(there->SMProwNumber,matrix,
                                        stat)==1) {
                                    /* gtri - modify - wbk - 4/30/91 - bug fix */
                                    if(jobs == jobs_max) {
                                        jobs_max *= 2;
                                        work = (int *) REALLOC(work, jobs_max * sizeof(int));
                                        printf("\nSMPpreOrder - 3 allocation of array -work- increased\n");
                                    }
                                    /* gtri - end - wbk - 4/30/91 - bug fix */
                                    work[jobs++]=there->SMProwNumber;
                                }
                            }
                            for(there = *(matrix->SMPcolHead+j);
                                    there != NULL; there = there->SMPcolNext) {
                                if(stat[there->SMProwNumber] == TODO &&
                                        SMPcount(there->SMProwNumber,matrix,
                                        stat)==1) {
                                    /* gtri - modify - wbk - 4/30/91 - bug fix */
                                    if(jobs == jobs_max) {
                                        jobs_max *= 2;
                                        work = (int *) REALLOC(work, jobs_max * sizeof(int));
                                        printf("\nSMPpreOrder 4 - allocation of array -work- increased\n");
                                    }
                                    /* gtri - end - wbk - 4/30/91 - bug fix */
                                    work[jobs++]=there->SMProwNumber;
                                }
                            }
                            /* finally do the swap we've been dreaming about */
                            SMProwSwap(matrix,i,j);
                            break;
                        }
                    }
                }
            }
/* gtri - modify - wbk - 5/1/91 - bug fix */
/*
            if(here==NULL) {
                FREE(stat);
                FREE(work);
                errMsg = MALLOC(strlen(badmsg)+1);
                strcpy(errMsg,badmsg);
                return(E_BADMATRIX);
            }
*/
            if(here==NULL) {
                FREE(stat);
                FREE(work);
                printf("\n%s\n", badmsg);
                printf("Attempting to continue...\n");
                return(OK);
            }
/*

Eliminated this test, since it causes simple circuits like the one shown below
to fail:

Title:Simple circuit with preorder problem
*
* This circuit fails with the message:
*
*  doAnalyses: Improperly formed matrix - shouldn't happen!(preOrd)
*
* The problem is caused by a combination of the zero-valued Vsource
* to ground at node 2, and the coincidence of e3's input and output
* negative connections at node 2.  This coincidence causes the
* symmetric pattern of 1 and -1 entries in the matrix for source e1
* to be destroyed whenever the gain of e1 is different from 0.0.
* Hence, the algorithm in SMPpreOrder() fails.
*
v1 1 0 1.0
r1 1 2 1K
v2 2 0 0.0
e1 (3 2) (1 2) 3.0
r2 3 2 1K
r3 3 0 1K
*
.op
.end

*/

/* gtri - end - wbk - 5/1/91 - bug fix */

        }
        /* ok - nothing left on the queue, so lets see if there are any
         * more possibly connected to each other but floating sources
         * that we need to deal with
         */
        for(i=0;i<matrix->SMPsize;i++) {
            if(stat[i]==TODO) {
                work[jobs++]=i;
                break;
            }
        }
    } while (jobs);
    FREE(stat);
    FREE(work);
    return(OK);
}
static int SMPcount(i,matrix,stat)
    register SMPmatrix *matrix;
    register char *stat;
    int i;

{
register SMPelement *here;
register SMPelement *there;
register int count;

    there = *(matrix->SMPcolHead+i);
    here = *(matrix->SMProwHead+i);
    count=0;
    while((here != NULL) && (there != NULL) ) {
        if((here->SMPcolNumber == there->SMProwNumber) && 
                ( here->SMPcolNumber != 0) &&
                ( here != there) ) {
            /* they are the same, check magnitude */
            if((FABS(here->SMPvalue) == 1) &&
                    (FABS(there->SMPvalue) == 1) ) {
                /* now to be sure we can use it! */
                if(stat[here->SMPcolNumber] != LOCK) {
                    /* found one! */
                    count++;
                }
            }
        } 
        if(here->SMPcolNumber < there->SMProwNumber) {
            /* col number less, so advance here */
            here= here->SMProwNext;
        } else {
            /* row number less, so advance there */
            there= there->SMPcolNext;
        }
    }
    return(count);
}
