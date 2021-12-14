/*
 * Copyright (c) 1988 Kanwar Jit Singh
 * singh@ic.Berkeley.edu
 */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "ASRCdefs.h"
#include "SPerror.h"
#include "util.h"
#include "suffix.h"

RCSID("ASRCconvTest.c $Revision: 1.1 $ on $Date: 90/10/11 12:40:25 $")

int
ASRCconvTest( inModel, ckt)
GENmodel *inModel;
CKTcircuit *ckt;
{
    register ASRCmodel *model = (ASRCmodel *)inModel;
    register ASRCinstance *here;
    int i, node_num, branch;
    double *vals, *derivs;
    double diff;
    double prev;
    double tol;
    double rhs;

    for( ; model != NULL; model = model->ASRCnextModel) {
        for( here = model->ASRCinstances; here != NULL;
                here = here->ASRCnextInstance){
            vals = (double *)MALLOC((here->ASRCtree->numVars)*sizeof(double));
            derivs = (double *)MALLOC((here->ASRCtree->numVars)*sizeof(double));

            for( i=0; i < here->ASRCtree->numVars; i++){
                if( here->ASRCtree->varTypes[i] == IF_INSTANCE){
                     branch = CKTfndBranch(ckt,here->ASRCtree->vars[i].uValue);
                     vals[i] = *(ckt->CKTrhsOld+branch);
                } else {
                    node_num = ((CKTnode *)(here->ASRCtree->vars[i].nValue))
                            ->number;
                    vals[i] = *(ckt->CKTrhsOld+node_num);
                }
            }

            if( (*(here->ASRCtree->IFeval))(here->ASRCtree,&rhs,
                    vals,derivs) == OK){

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
/* gtri - begin - wbk - report conv prob */
                    if(ckt->enh->conv_debug.report_conv_probs) {
                        ENHreport_conv_prob(ENH_ANALOG_INSTANCE,
                                            (char *) here->ASRCname,
                                            "");
                    }
/* gtri - end - wbk - report conv prob */
                    ckt->CKTnoncon++;
                    return(OK);
                }
            } else {
                return(E_BADPARM);
            }
        }
    }
    return(OK);
}
