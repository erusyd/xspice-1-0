/*
 * Copyright (c) 1985 Mani B. srivastava
 */

    /* NIpzMuller(ckt,ptrlistptr,type)
     *This is a routine implementing the Muller's method of 
     *finding roots of a complex polynomial
     *ptrlistptr is the pointer to a pointer of a list of roots
     *type is a number reflecting the type of analysis being done
     */

#include "prefix.h"
#include <stdio.h>
#include "PZdefs.h"
#include <math.h>
#include "util.h"
#include "complex.h"
#include "CKTdefs.h"
#include "DEVdefs.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("NIpzMuller.c $Revision: 1.1 $ on $Date: 91/04/02 11:47:31 $")

int
NIpzMuller(ckt,ptrlistptr,type)
    register CKTcircuit *ckt;
    root **ptrlistptr;
    int type;
{
    int itr,error,NPASS;
    int power0,power1,power2,power3,power,scale;
    int oldscale,newscale;
    int scale1,scale2,scale0;
    int NIpzSolve();
    int first,num;
    double test;
    double temp1,temp2;
    double XMAX,PZTOL,PZABS,NMAX,NROOT,MATSIZE;
    double xmag,hmag,amax1,xi,xr;
    SPcomplex x0,x1,x2,x3,x;
    SPcomplex fx0,fx1,fx2,fx3,fx;
    SPcomplex h2,h1,h;
    SPcomplex lambda,lambda2;
    SPcomplex droot,denom1,denom;
    SPcomplex delta2;
    SPcomplex a,b,c;
    SPcomplex ctemp0,ctemp1,ctemp2,ctemp3,ctemp4,ctemp5;
    SPcomplex x2minusx,x1minusx,x0minusx;
    root *temproot;
    root *listptr;
    static char *itmsg = "Too many iterations without convergence";
    static char *magmsg = "Magnitude overflow in pole-zero analysis";
    /*
    printf("MULLER:entering \n");
    */
    listptr=(root *)(NULL);
    XMAX=1.0e20;
    PZTOL=1.0e-6;
    PZABS=1.0e-6;
    NPASS=1;
    NMAX=50;
    NROOT=0;
    MATSIZE=SMPmatSize(ckt->CKTmatrix);

    /*choose initial values x0 , x1 , x2 */
    lp100:
    first=0;
    scale=0;
    oldscale=0;
    x0.real= 1.763e-3;
    x0.imag=0.0;
    x1.real= -1.391e-5;
    x1.imag=0.0;
    x2.real= -1.271e0;
    x2.imag=0.0;
    /*calculate f(x0) , f(x1) , f(x2) 
     *note that f(s)=( det[Y(s)])/((s-r1)*...*(s-rk))
     *where r1,r2,r3...rk are the roots that have been found
     */
    error=NIpzSolve(ckt,&x2,listptr,&fx2,type,&power2);
    if (error) return(error);
    error=NIpzSolve(ckt,&x1,listptr,&fx1,type,&power1);
    if (error) return(error);
    error=NIpzSolve(ckt,&x0,listptr,&fx0,type,&power0);
    if (error) return(error);
    /*start search for a new root*/
    lp170:
    itr=3;
    lp175:
    /*first find a scaling factor for better numerical calculation*/
    if ((fabs(fx2.real)>0.0) || (fabs(fx2.imag)>0.0) ||
            (fabs(fx1.real)>0.0) || (fabs(fx2.imag)>0.0) ||
            (fabs(fx0.real)>0.0) || (fabs(fx2.imag)>0.0)) {
        newscale=0;
        num=0;
        if ((fabs(fx2.real)>0.0) || (fabs(fx2.imag)>0.0)) {
             /* printf("f(0)=(%5.6e,%5.6e) \n",fx2.real,fx2.imag); */
             scale2=1-power2;
             newscale=scale2;
             num++;
        }
        if ((fabs(fx1.real)>0.0) || (fabs(fx2.imag)>0.0)) {
             /* printf("f(0)=(%5.6e,%5.6e) \n",fx1.real,fx1.imag); */
             scale1=1-power1;
             num++;
             newscale=(newscale+scale1)/num;
        }
        if ((fabs(fx0.real)>0.0) || (fabs(fx2.imag)>0.0)) {
             /* printf("f(0)=(%5.6e,%5.6e) \n",fx0.real,fx0.imag); */
             scale0=1-power0;
             num++;
             newscale=((newscale*(num-1))+scale0)/num;
        }
     } else if (NPASS==1) {
        x3.real=1.0;
        x3.imag=0.0;
        AGAIN:
        error=NIpzSolve(ckt,&x3,listptr,&fx3,type,&power);
        if (error) return(error);
        /* printf("f(0)=(%5.6e,%5.6e) \n",fx3.real,fx3.imag); */
        if (fabs(fx3.real)>0.0) {
            newscale=1-power;
        } else {
            x3.real=x3.real+1.0;
            goto AGAIN;
        };
    };
    /* printf("newscale=%5.d \n",newscale); */
    scale=newscale-oldscale;
    oldscale=newscale;
    /* printf("scale=%5.d \n",scale); */
    if (first==0) {
        if ((fx2.real != 0.0) || (fx2.imag != 0.0)) {
            fx2.real=(pow(10.0,(double)(power2+scale))) * fx2.real;
            fx2.imag=(pow(10.0,(double)(power2+scale))) * fx2.imag;
        };
        if ((fx1.real != 0.0) || (fx1.imag != 0.0)) {
            fx1.real=(pow(10.0,(double)(power1+scale))) * fx1.real;
            fx1.imag=(pow(10.0,(double)(power1+scale))) * fx1.imag;
        };
        if ((fx0.real != 0.0) || (fx0.imag != 0.0)) {
            fx0.real=(pow(10.0,(double)(power0+scale))) * fx0.real;
            fx0.imag=(pow(10.0,(double)(power0+scale))) * fx0.imag;
        };
        first=1;
    } else {
        if ((fx2.real != 0.0) || (fx2.imag != 0.0)) {
            fx2.real=(pow(10.0,(double)(scale))) * fx2.real;
            fx2.imag=(pow(10.0,(double)(scale))) * fx2.imag;
        };
        if ((fx1.real != 0.0) || (fx1.imag != 0.0)) {
            fx1.real=(pow(10.0,(double)(scale))) * fx1.real;
            fx1.imag=(pow(10.0,(double)(scale))) * fx1.imag;
        };
        if ((fx0.real != 0.0) || (fx0.imag != 0.0)) {
            fx0.real=(pow(10.0,(double)(scale))) * fx0.real;
            fx0.imag=(pow(10.0,(double)(scale))) * fx0.imag;
        };
    };
    /* printf("x0=(%5.6e,%5.6e) \n",x0.real,x0.imag); */
    /* printf("fx0=(%5.6e,%5.6e) \n",fx0.real,fx0.imag); */
    /* printf("x1=(%5.6e,%5.6e) \n",x1.real,x1.imag); */
    /* printf("fx1=(%5.6e,%5.6e) \n",fx1.real,fx1.imag); */
    /* printf("x2=(%5.6e,%5.6e) \n",x2.real,x2.imag); */
    /* printf("fx2=(%5.6e,%5.6e) \n",fx2.real,fx2.imag); */

    /*now check whether f(x) is nearly a constant => all roots
     *have been found
     *in fact if this happens in any iteration , it means that
     *all roots have been found and we exit
     */
     /*calculate temp1=|f(x2)-f(x0)|*/
    temp1=fabs(fx2.real-fx0.real)+fabs(fx2.imag-fx0.imag);
    /* printf("temp1=%5.6e \n",temp1); */
    /*calculate temp2=|f(x2)-f(x1)|*/
    temp2=fabs(fx2.real-fx1.real)+fabs(fx2.imag-fx1.imag);
    /* printf("temp2=%5.6e\n",temp2); */
    /*check whether fx2,fx1,fx0 are nearly equal*/
    if ((temp1+temp2) > (PZTOL*fabs(fx2.real))) {
        goto lp200;
    };
    x3.real=100.0*x0.real;
    x3.imag=100.0*x0.imag;
    error=NIpzSolve(ckt,&x3,listptr,&fx3,type,&power3);
    if (error) return(error);
    if ((fx3.real != 0.0) || (fx3.imag != 0.0)) {
        fx3.real=(pow(10.0,(double)(power3+newscale))) * fx3.real;
        fx3.imag=(pow(10.0,(double)(power3+newscale))) * fx3.imag;
    };
    /*calculate temp1=|f(x2)-f(x3)|*/
    temp1=fabs(fx2.real-fx3.real)+fabs(fx2.imag-fx3.imag);
    /* printf("temp1a=%5.6e\n",temp1); */
    /* printf("temp2a=%5.6e\n",temp2); */
    if ((temp1+temp2) <= (PZTOL*fabs(fx2.real))) {
        goto lp1000;
    };
lp200:
    if (NROOT>MATSIZE) goto lp1000;
    /*calculate h2=x2-x1*/
    h2.real=x2.real - x1.real ;
    h2.imag=x2.imag - x1.imag ;
    /*calculate h1=x1-x0*/
    h1.real=x1.real - x0.real ;
    h1.imag=x1.imag - x0.imag ;
    /*calculate lambda2=h2 / h1*/
    lambda2.real = h2.real;
    lambda2.imag = h2.imag;
    DC_DIVEQ( &(lambda2.real) , &(lambda2.imag) , h1.real , h1.imag ) ;
    /*calculate delta2=1 + lambda2*/
    delta2.real=1.0+lambda2.real;
    delta2.imag=lambda2.imag;
    /*calculate ctemp0=lambda2*lambda2*/
    DC_MULT(lambda2.real , lambda2.imag , lambda2.real ,
        lambda2.imag , &(ctemp0.real) , &(ctemp0.imag));
    /*calculate ctemp1=fx0 * lambda2 * lambda2*/
    DC_MULT(fx0.real , fx0.imag , ctemp0.real , ctemp0.imag ,
        &(ctemp1.real) , &(ctemp1.imag));
    /*calculate ctemp2=fx1 * delta2*/
    DC_MULT(fx1.real , fx1.imag , delta2.real , delta2.imag ,
        &(ctemp2.real) , &(ctemp2.imag));
    /*calculate ctemp3=fx2 * lambda2*/
    DC_MULT(fx2.real , fx2.imag , lambda2.real , lambda2.imag ,
        &(ctemp3.real) , &(ctemp3.imag));
    /*calculate ctemp4=fx1 * delta2 * lambda2*/
    DC_MULT(ctemp2.real , ctemp2.imag , lambda2.real , lambda2.imag ,
        &(ctemp4.real) , &(ctemp4.imag));
    /*calculate a=fx0 * lambda2 ^ 2 
     *            - fx1 * lambda2 * delta2
     *            + fx2 * lambda2
     */
    a.real=ctemp1.real+ctemp3.real-ctemp4.real;
    a.imag=ctemp1.imag+ctemp3.imag-ctemp4.imag;
    /*calculate c=fx2 * delta2*/
    c.real=fx2.real+ctemp3.real;
    c.imag=fx2.imag+ctemp3.imag;
    /*calculate b=fx0 * lambda2 ^ 2 
     *            - fx1 * delta2 ^ 2
     *            + fx2 * lambda2
     *            + fx2 * delta2
     */
    b.real=ctemp1.real-ctemp2.real+ctemp3.real-ctemp4.real+c.real;
    b.imag=ctemp1.imag-ctemp2.imag+ctemp3.imag-ctemp4.imag+c.imag;
    /*calculate ctemp5=b^2 - 4 * a * c */
    ctemp5.real=b.real * b.real - b.imag * b.imag -
        4.0 * a.real * c.real + 4.0 * a.imag * c.imag;
    ctemp5.imag=2.0 * b.real * b.imag -
        4.0 * a.real * c.imag - 4.0 * a.imag * c.real;
/*now find the complex square root of ctemp5*/
    if (ctemp5.real == 0.0) {
        if (ctemp5.imag == 0.0) {
            droot.real = 0.0;
            droot.imag = 0.0;
        } else if (ctemp5.imag > 0.0) {
            droot.real = sqrt (0.5 * ctemp5.imag);
            droot.imag = droot.real;
        } else { 
            droot.imag = sqrt( -0.5 * ctemp5.imag);
            droot.real= - droot.imag;
        }
    } else if (ctemp5.real > 0.0) {
        if (ctemp5.imag == 0.0) {
            droot.real = sqrt(ctemp5.real);
            droot.imag = 0.0;
        } else if (ctemp5.imag < 0.0) {
            droot.real = -sqrt(0.5 * (sqrt(ctemp5.real * ctemp5.real
                    + ctemp5.imag * ctemp5.imag) + ctemp5.real));
        } else {
            droot.real =  sqrt(0.5 * (sqrt(ctemp5.real * ctemp5.real
                + ctemp5.imag * ctemp5.imag) + ctemp5.real));
        }
        droot.imag = ctemp5.imag / (2.0 * droot.real);
    } else { /* ctemp5.real < 0.0) */
        if (ctemp5.imag == 0.0) {
            droot.real = 0.0;
            droot.imag = sqrt(- ctemp5.real);
        } else {
            if (ctemp5.imag < 0.0) {
                droot.imag = -sqrt(0.5 * (sqrt(ctemp5.real * ctemp5.real
                        + ctemp5.imag * ctemp5.imag) - ctemp5.real));
            } else {
                droot.imag =  sqrt(0.5 * (sqrt(ctemp5.real * ctemp5.real
                        + ctemp5.imag * ctemp5.imag) - ctemp5.real));
                droot.real = ctemp5.imag / (2.0 * droot.imag);
            }
        }
    };
    /*calculate denom=b+sqrt(b^2-4ac) */
    denom.real=b.real + droot.real;
    denom.imag=b.imag + droot.imag;
    /*calculate denom1=b-sqrt(b^2-4ac) */
    denom1.real=b.real - droot.real;
    denom1.imag=b.imag - droot.imag;
    if ((fabs(denom1.real) + fabs(denom1.imag)) > 
            (fabs(denom.real) + fabs(denom.imag))) {
        denom.real = denom1.real;
        denom.imag = denom1.imag;
    };
    /*calculate smallest zero lambda = -2c/denom */
    lambda.real= -2.0 * c.real;
    lambda.imag= -2.0 * c.imag;
    DC_DIVEQ(&(lambda.real) , &(lambda.imag) , denom.real , denom.imag);
    /*calculate h=lambda * h2 */
    DC_MULT(lambda.real , lambda.imag , h2.real , h2.imag ,&(h.real),&(h.imag));
    /*calculate new x=x2 + h */
    x.real=x2.real + h.real;
    x.imag=x2.imag + h.imag;

    /*do convergence test*/
    xmag=fabs(x.real) + fabs(x.imag);
    if (xmag > XMAX) {
        if (NPASS >= 2) {
            errMsg = MALLOC(strlen(magmsg)+1);
            strcpy(errMsg,magmsg);
            return(E_MAGEXCEEDED);
        } else {
            NPASS++;
            goto lp100;
        };
    };
    hmag=fabs(h.real) + fabs(h.imag);
    amax1=(xmag>PZABS) ? xmag : PZABS ;
    if (hmag <= PZTOL * amax1) goto lp300 ;
    error=NIpzSolve(ckt,&x,listptr,&fx,type,&power);
    if (error) return(error);
    if ((fx.real != 0.0) || (fx.imag != 0.0)) {
        fx.real=(pow(10.0,(double)(power+newscale))) * fx.real;
        fx.imag=(pow(10.0,(double)(power+newscale))) * fx.imag;
    } else {
        /*fx=(0,0)*/
        power=0;
    };
    /* printf("x=(%5.6e,%5.6e) \n",x.real,x.imag); */
    /* printf("fx=(%5.6e,%5.6e) \n",fx.real,fx.imag); */
    if ((fabs(fx.real) + fabs(fx.imag)) < 1.0e-25) {
        goto lp300 ; /*converged as |f(x2)|<1.0e-30 so that x2
               is a root*/
    };

    /*no convergence*/
    if (itr >= NMAX) {
        errMsg = MALLOC(strlen(itmsg)+1);
        strcpy(errMsg,itmsg);
        return(E_ITERLIM) ;
    }
    itr +=1 ;
    fx0.real = fx1.real ;
    fx0.imag = fx1.imag ;
    x0.real  = x1.real  ;
    x0.imag  = x1.imag  ;
    power0=power1;
    fx1.real = fx2.real ;
    fx1.imag = fx2.imag ;
    x1.real  = x2.real  ;
    x1.imag  = x2.imag  ;
    power1=power2;
    fx2.real = fx.real ;
    fx2.imag = fx.imag ;
    x2.real  = x.real  ;
    x2.imag  = x.imag  ;
    power2=power;
    goto lp175 ;

    /*converged*/
lp300:
    xr=x.real ;
    xi=x.imag ;
    xi=(fabs(xi) < (PZTOL * fabs(xr))) ? 0.0 : xi ;
    xi=(fabs(xi) < 1.0e-15) ? 0.0 : xi ;
    xr=(fabs(xr) < (PZTOL * fabs(xi))) ? 0.0 : xr ;
    xr=(fabs(xr) < 1.0e-15) ? 0.0 : xr ;
    /* printf("root=(%5.6e,%5.6e) \n",xr,xi); */
    NROOT++ ;
    /*store the root*/
    temproot=(root *) malloc((unsigned) sizeof(root));
    if (temproot==(root *)(NULL)) return(E_NOMEM);
    temproot->real=xr;
    temproot->imag=xi;
    temproot->next=listptr;
    listptr=temproot;
    x2minusx.real=x2.real-x.real;
    x2minusx.imag=x2.imag-x.imag;
    x1minusx.real=x1.real-x.real;
    x1minusx.imag=x1.imag-x.imag;
    x0minusx.real=x0.real-x.real;
    x0minusx.imag=x0.imag-x.imag;
    DC_DIVEQ(&(fx2.real),&(fx2.imag),x2minusx.real,x2minusx.imag);
    DC_DIVEQ(&(fx1.real),&(fx1.imag),x1minusx.real,x1minusx.imag);
    DC_DIVEQ(&(fx0.real),&(fx0.imag),x0minusx.real,x0minusx.imag);
    if (xi==0.0) goto lp400;
    /*store the conjugate root*/
    NROOT++ ;
    temproot=(root *) malloc((unsigned) sizeof(root));
    if (temproot==(root *)(NULL)) return(E_NOMEM);
    temproot->real=xr;
    temproot->imag= -xi;
    /* printf("root=(%5.6e,%5.6e) \n",xr,-xi); */
    temproot->next=listptr;
    listptr=temproot;
    x2minusx.real=x2.real-x.real;
    x2minusx.imag=x2.imag+x.imag;
    x1minusx.real=x1.real-x.real;
    x1minusx.imag=x1.imag+x.imag;
    x0minusx.real=x0.real-x.real;
    x0minusx.imag=x0.imag+x.imag;
    DC_DIVEQ(&(fx2.real),&(fx2.imag),x2minusx.real,x2minusx.imag);
    DC_DIVEQ(&(fx1.real),&(fx1.imag),x1minusx.real,x1minusx.imag);
    DC_DIVEQ(&(fx0.real),&(fx0.imag),x0minusx.real,x0minusx.imag);
    lp400:
    /*normalise fx2*/
    test=(fabs(fx2.real)>fabs(fx2.imag)) ?
    fabs(fx2.real) : fabs(fx2.imag) ;
    num=0;
    if (test !=0.0) {
        while (test>1.0) {
            test=test/10.0;
            fx2.real=fx2.real/10.0;
            fx2.imag=fx2.imag/10.0;
            num=num + 1;
        };
        while (test<=0.1) {
            test=test*10.0;
            fx2.real=fx2.real*10.0;
            fx2.imag=fx2.imag*10.0;
            num=num - 1;
        };
    };
    power2=num-oldscale;
    /*normalise fx1*/
    test=(fabs(fx1.real)>fabs(fx1.imag)) ?
    fabs(fx1.real) : fabs(fx1.imag) ;
    num=0;
    if (test !=0.0) {
        while (test>1.0) {
            test=test/10.0;
            fx1.real=fx1.real/10.0;
            fx1.imag=fx1.imag/10.0;
            num=num + 1;
        };
        while (test<=0.1) {
            test=test*10.0;
            fx1.real=fx1.real*10.0;
            fx1.imag=fx1.imag*10.0;
            num=num - 1;
        };
    };
    power1=num-oldscale;
    /*normalise fx0*/
    test=(fabs(fx0.real)>fabs(fx0.imag)) ?
    fabs(fx0.real) : fabs(fx0.imag) ;
    num=0;
    if (test !=0.0) {
        while (test>1.0) {
            test=test/10.0;
            fx0.real=fx0.real/10.0;
            fx0.imag=fx0.imag/10.0;
            num=num + 1;
        };
        while (test<=0.1) {
            test=test*10.0;
            fx0.real=fx0.real*10.0;
            fx0.imag=fx0.imag*10.0;
            num=num - 1;
        };
    };
    power0=num-oldscale;
    first=0;
    oldscale=0;
    goto lp170;
    
lp1000:
    *ptrlistptr=listptr;
    return(OK);
}
