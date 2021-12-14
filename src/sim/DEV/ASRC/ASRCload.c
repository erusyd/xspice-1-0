/*
 * Copyright (c) 1987 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "ASRCdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("ASRCload.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:27 $")

/*ARGSUSED*/
int
ASRCload(inModel,ckt)
GENmodel *inModel;
CKTcircuit *ckt;
{

    /* actually load the current voltage value into the 
     * sparse matrix previously provided 
     */

    register ASRCmodel *model = (ASRCmodel*)inModel;
    register ASRCinstance *here;
    int i, v_first, j, branch;
    int node_num;
    int size;
    double *vals, *derivs;
    double rhs;
    double prev;
    double diff;
    double tol;

    /*  loop through all the Arbitrary source models */
    for( ; model != NULL; model = model->ASRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->ASRCinstances; here != NULL ;
                here=here->ASRCnextInstance) {
            
        /*
         * Get the function and its derivatives evaluated 
         */
        v_first = 1;
        j=0;
        vals = (double *)
            MALLOC((here->ASRCtree->numVars)*sizeof(double));
        derivs = (double *)
            MALLOC((here->ASRCtree->numVars)*sizeof(double));

        /*
         * Fill the vector of values from the previous solution
         */
        for( i=0; i < here->ASRCtree->numVars; i++){
            if( here->ASRCtree->varTypes[i] == IF_INSTANCE){
             branch = CKTfndBranch(ckt,
                here->ASRCtree->vars[i].uValue);
             vals[i] = *(ckt->CKTrhsOld+branch);
            } else {
            node_num = ((CKTnode *)(here->ASRCtree->vars[i].
            nValue))->number;
            vals[i] = *(ckt->CKTrhsOld+node_num);
            }
        }

        if( (*(here->ASRCtree->IFeval))(here->ASRCtree,&rhs,
            vals,derivs) == OK){

        /* The convergence test */

        if ( (ckt->CKTmode & MODEINITFIX) || 
            (ckt->CKTmode & MODEINITFLOAT) ){
#ifndef NEWCONV
            prev = here->ASRCprev_value;
            diff = FABS( prev - rhs);
            if ( here->ASRCtype == ASRC_VOLTAGE){
            tol = ckt->CKTreltol *
            MAX(FABS(rhs),FABS(prev)) + ckt->CKTvoltTol;
            } else {
            tol = ckt->CKTreltol *
            MAX(FABS(rhs),FABS(prev)) + ckt->CKTabstol;
            }

            if ( diff > tol) {
            ckt->CKTnoncon++;
            }
#endif /* NEWCONV */
        }
        here->ASRCprev_value = rhs;

        /* The ac load precomputation and storage */

        if (ckt->CKTmode & MODEINITSMSIG){
            size = (here->ASRCtree->numVars)+1 ;
            here->ASRCacValues = (double *)MALLOC(size*sizeof(double));
            for ( i = 0; i < here->ASRCtree->numVars; i++){
            here->ASRCacValues[i] = derivs[i]; 
            }
        }

        for(i=0; i < here->ASRCtree->numVars; i++){
            rhs -= (vals[i] * derivs[i]);
            switch(here->ASRCtree->varTypes[i]){
            case IF_INSTANCE:
            if( here->ASRCtype == ASRC_VOLTAGE){
            /* CCVS */
                if(v_first){
                *(here->ASRCposptr[j++]) += 1.0;
                *(here->ASRCposptr[j++]) -= 1.0;
                *(here->ASRCposptr[j++]) -= 1.0;
                *(here->ASRCposptr[j++]) += 1.0;
                v_first = 0;
               }
        *(here->ASRCposptr[j++]) -= derivs[i];
            } else{
        /* CCCS */
        *(here->ASRCposptr[j++]) += derivs[i];
        *(here->ASRCposptr[j++]) -= derivs[i];
            }
            break;
            case IF_NODE:
            if(here->ASRCtype == ASRC_VOLTAGE){
        /* VCVS */
                if( v_first){
                *(here->ASRCposptr[j++]) += 1.0;
                *(here->ASRCposptr[j++]) -= 1.0;
                *(here->ASRCposptr[j++]) -= 1.0;
                *(here->ASRCposptr[j++]) += 1.0;
                v_first = 0;
                }
        *(here->ASRCposptr[j++]) -= derivs[i];
            } else {
        /*VCCS*/
                *(here->ASRCposptr[j++]) += derivs[i];
                *(here->ASRCposptr[j++]) -= derivs[i];
            }
            break;
            default:
            return(E_BADPARM);
            }
        }
        /* Insert the RHS */
        if( here->ASRCtype == ASRC_VOLTAGE){
            *(ckt->CKTrhs+(here->ASRCbranch)) += rhs;
        } else {
            *(ckt->CKTrhs+(here->ASRCposNode)) -= rhs;
            *(ckt->CKTrhs+(here->ASRCnegNode)) += rhs;
        }

        /* Store the rhs for small signal analysis */
        if (ckt->CKTmode & MODEINITSMSIG){
            here->ASRCacValues[here->ASRCtree->numVars] = rhs; 
        }
            } else{
                return(E_BADPARM);
            }
        }
    }
    FREE(vals);
    FREE(derivs);
    return(OK);
}
