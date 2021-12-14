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
#include "complex.h"

RCSID("ASRCpzLoad.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:29 $")

/*ARGSUSED*/
int
ASRCpzLoad(inModel,ckt,s)

    GENmodel *inModel;
    CKTcircuit *ckt;
    SPcomplex *s;

        /* actually load the current voltage value into the 
         * sparse matrix previously provided 
         */
{
    register ASRCmodel *model = (ASRCmodel*)inModel;
    register ASRCinstance *here;
    int i, v_first, j=0, branch;
    int node_num;
    double value, *vals, *derivs;

    /*  loop through all the Arbitrary source models */
    for( ; model != NULL; model = model->ASRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->ASRCinstances; here != NULL ;
                here=here->ASRCnextInstance) {
            
            /* Get the function evaluated and the derivatives too */
            v_first = 1;
            vals = (double *)MALLOC((here->ASRCtree->numVars) * sizeof(double));
            derivs=(double *)MALLOC((here->ASRCtree->numVars) * sizeof(double));

            /* Fill the vector of values from the previous solution */
            for( i=0; i < here->ASRCtree->numVars; i++){
                if( here->ASRCtree->varTypes[i] == IF_INSTANCE){
                     branch = CKTfndBranch(ckt,here->ASRCtree->vars[i].uValue);
                     vals[i] = *(ckt->CKTrhsOld+branch);
                } else {
                    node_num = ((CKTnode *)(here->ASRCtree->vars[i].nValue))->
                            number;
                    vals[i] = *(ckt->CKTrhsOld+node_num);
                }
            }

            if( (*(here->ASRCtree->IFeval))(here->ASRCtree,&value,
                    vals,derivs) == OK){
                for(i=0; i < here->ASRCtree->numVars; i++){
                    switch(here->ASRCtree->varTypes[i]){
                        case IF_INSTANCE:
                            if( here->ASRCtype == ASRC_VOLTAGE){
                                /* CCVS */
                                if(v_first){
                                    *(here->ASRCposptr[j++]) += 1.0;
                                    *(here->ASRCposptr[j++]) -= 1.0;
                                    *(here->ASRCposptr[j++]) -= 1.0;
                                    *(here->ASRCposptr[j++]) += 1.0;
                                    *(here->ASRCposptr[j++]) -= derivs[i];
                                    v_first = 0;
                                } else {
                                    *(here->ASRCposptr[j++]) -= derivs[i];
                                }
                            } else {
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
                                    *(here->ASRCposptr[j++]) -= derivs[i];
                                    v_first = 0;
                                } else {
                                    *(here->ASRCposptr[j++]) -= derivs[i];
                                }
                            } else {
                /* VCCS */
                                *(here->ASRCposptr[j++]) += derivs[i];
                                *(here->ASRCposptr[j++]) -= derivs[i];
                            }
                            break;
                        default:
                            return(E_BADPARM);
                    }
                }
            } else {
                return(E_BADPARM);
            }
        }
    }
    FREE(derivs);
    FREE(vals);
    return(OK);
}
