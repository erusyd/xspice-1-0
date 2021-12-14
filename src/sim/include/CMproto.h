#ifndef CMPROTO_DEFINED
#define CMPROTO_DEFINED

/* ===========================================================================
FILE    CMproto.h

MEMBER OF process XSPICE

Copyright 1991
Georgia Tech Research Corporation
Atlanta, Georgia 30332
All Rights Reserved

PROJECT A-8503

AUTHORS

    9/12/91  Jeff Murray, Bill Kuhn

MODIFICATIONS

    <date> <person name> <nature of modifications>

SUMMARY

    This file contains ANSI C function prototypes for cm_xxx functions
    called by code models.

INTERFACES

    None.

REFERENCED FILES

    None.

NON-STANDARD FEATURES

    None.

=========================================================================== */

/* Prototypes for functions used by internal code models */
/* The actual functions reside in ../ICM/CMutil.c        */
/*                                         12/17/90      */


#include "CMtypes.h"


void cm_climit_fcn(double in, double in_offset, double cntl_upper, 
                   double cntl_lower, double lower_delta, 
                   double upper_delta, double limit_range, 
                   double gain, int percent, double *out_final,
                   double *pout_pin_final, double *pout_pcntl_lower_final,
                   double *pout_pcntl_upper_final);



void cm_smooth_corner(double x_input, double x_center, double y_center,
                 double domain, double lower_slope, double upper_slope,
                 double *y_output, double *dy_dx);
void cm_smooth_discontinuity(double x_input, double x_lower, double y_lower,
                 double x_upper, double y_upper, 
                 double *y_output, double *dy_dx);
double cm_smooth_pwl(double x_input, double *x, double *y, int size,
               double input_domain, double *dout_din);

double cm_analog_ramp_factor(void);
void *cm_analog_alloc(int tag, int bytes);
void *cm_analog_get_ptr(int tag, int timepoint);
int  cm_analog_integrate(double integrand, double *integral, double *partial);
int  cm_analog_converge(double *state);
int  cm_analog_set_temp_bkpt(double time);
int  cm_analog_set_perm_bkpt(double time);
void cm_analog_not_converged(void);
void cm_analog_auto_partial(void);

void *cm_event_alloc(int tag, int bytes);
void *cm_event_get_ptr(int tag, int timepoint);
int  cm_event_queue(double time);

char *cm_message_get_errmsg(void);
int  cm_message_send(char *msg);

double cm_netlist_get_c(void);
double cm_netlist_get_l(void);

Complex_t cm_complex_set(double real, double imag);
Complex_t cm_complex_add(Complex_t x, Complex_t y);
Complex_t cm_complex_subtract(Complex_t x, Complex_t y);
Complex_t cm_complex_multiply(Complex_t x, Complex_t y);
Complex_t cm_complex_divide(Complex_t x, Complex_t y);

#endif /* CMPROTO_DEFINED */