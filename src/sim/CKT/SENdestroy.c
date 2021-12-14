/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SENdestroy(ckt)
     * this is a driver program to iterate through all the various
     * destroy functions provided for the circuit elements in the
     * given circuit 
     */

#include "prefix.h"
#include <stdio.h>
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "util.h"
#include "IFsim.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SENdestroy.c $Revision: 1.1 $ on $Date: 91/04/02 12:08:49 $")

void
SENdestroy(info)
    SENstruct *info;
{
    register int i;
    int size;


     size = info->SENsize;
#ifdef SENSDEBUG
     printf("size = %d\n",size);
     printf("freeing sensitivity structure in SENdestroy\n");
#endif /* SENSDEBUG */
     /*
     if(info->SENdevices) FREE(info->SENdevices);
     if(info->SENparmNames) FREE(info->SENparmNames);
     */
         if(info->SEN_Sap){
#ifdef SENSDEBUG
     printf("freeing SEN_Sap in SENdestroy\n");
#endif /* SENSDEBUG */
               for(i=0;i<=size;i++){
              if(info->SEN_Sap[i]) FREE(info->SEN_Sap[i]);
               }
           FREE(info->SEN_Sap);
         }
     if(info->SEN_RHS){
               for(i=0;i<=size;i++){
              if(info->SEN_RHS[i]) FREE(info->SEN_RHS[i]);
               }
           FREE(info->SEN_RHS);
         }
         if(info->SEN_iRHS){
               for(i=0;i<=size;i++){
              if(info->SEN_iRHS[i]) FREE(info->SEN_iRHS[i]);
               }
           FREE(info->SEN_Sap);
         }
     /*
     FREE(info);
     */
#ifdef SENSDEBUG
     printf("SENdestroy end\n");
#endif /* SENSDEBUG */

    return;
}
