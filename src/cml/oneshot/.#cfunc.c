#include "CM.h"
#include "oneshot.h"  /* ...generated from IF spec. table..? Contains */
                     /* any model-specific defns.      10/10/90 JPM  */


/**********************************************************************
*
*  This model describes a totally analog oneshot.
*  After a rising edge is detected, the model will
*  output a pulse width specified by the controling 
*  voltage.
*                          HWL 20Mar91
*
*
*
*                              ___________________________________
*                             /<---pulse width --->              :\
*                            / :                  :              : \ 
*         <---rise_delay--> /  :                  :<-fall_delay->:  \
*      ___|________________/   :                  :              :   \____________
*         ^                <-->:                  :              :<-->
*         Trigger         Risetime                               Falltime
*
*
************************************************************************/

void cm_oneshot(Mif_Private_t *private)  /* structure holding parms, 
                                   inputs, outputs, etc.     */
{
    int i;             /* generic loop counter index                    */
	int cntl_size;     /* size of the control array                     */
	int pw_size;       /* size of the pulse-width array                 */
	int *state;        /* pointer used to store state1 variable         */
	int state1;        /* if state1 = 1, then oneshot has
                          been triggered.  if state1 = 0, no change     */
	int *set;          /* pointer used to store the state of set1       */
	int set1;          /* flag used to set/reset the oneshot            */
    int trig_pos_edge; /* flag used to define positive or negative
                          edge triggering.  1=positive, 0=negative      */

    double *x;         /* pointer used to store the control array       */
	double *y;         /* pointer used to store the pulse-width array   */
	double cntl_input; /* the actual value of the control input         */
	double out;        /* value of the output                           */
	double output_low; /* output low value                              */
	double output_hi;  /* output high value                             */
    double pw;         /* actual value of the pulse-width               */
/*	double del_out;     value of the delay time between triggering 
                          and a change in the output                    */
	double del_rise;    /* value of the delay time between triggering 
                          and a change in the output                    */
	double del_fall;    /* value of the delay time between the end of the
						   pw and a change in the output                */
	double *t1;        /* pointer used to store time1                   */
	double *t2;        /* pointer used to store time2                   */
	double *t3;        /* pointer used to store time3                   */
	double *t4;        /* pointer used to store time4                   */
	double time1;      /* time at which the output first begins to 
                          change (trigger + delay)                      */
	double time2;      /* time2 = time1 + risetime                      */
	double time3;      /* time3 = time2 + pw                            */
	double time4;      /* time4 = time3 + falltime                      */
	double t_rise;     /* risetime                                      */
	double t_fall;     /* falltime                                      */
	double *output_old;/* pointer which stores the previous output      */
    double *clock;     /* pointer which stores the clock                */
	double *old_clock; /* pointer which stores the previous clock       */
	double trig_clk;   /* value at which the clock triggers the oneshot */

    Mif_Complex_t ac_gain;
                                           

    /**** Retrieve frequently used parameters... ****/

    cntl_size = private->param[0]->size;           
    pw_size = private->param[1]->size;           
    trig_clk = private->param[2]->element[0].rvalue;
    trig_pos_edge = private->param[3]->element[0].bvalue;
    output_low = private->param[4]->element[0].rvalue;
    output_hi = private->param[5]->element[0].rvalue;
    /*del_out = PARAM(delay);*/
	del_rise = private->param[7]->element[0].rvalue;
	del_fall = private->param[8]->element[0].rvalue;
    t_rise = private->param[6]->element[0].rvalue;
    t_fall = private->param[9]->element[0].rvalue;

    /* set minimum rise and fall_times */

	if(t_rise < 1e-12){
		t_rise = 1e-12;
    }

	if(t_fall < 1e-12){
		t_fall = 1e-12;
    }

   /* the control array must be the same size as the pulse-width array */ 

	if(cntl_size != pw_size){
		cm_send_error_msg(oneshot_array_error);
		return;
 	}

  if(private->circuit.init == 1){  /* first time through, allocate memory */

		t1 = cm_alloc(T1,sizeof(double));
		t2 = cm_alloc(T2,sizeof(double));
		t3 = cm_alloc(T3,sizeof(double));
		t4 = cm_alloc(T4,sizeof(double));
        set = cm_alloc(SET,sizeof(int));
        state = cm_alloc(STATE,sizeof(int));
        clock = cm_alloc(CLOCK,sizeof(double));
        output_old = cm_alloc(OUTPUT_OLD,sizeof(double));

  } 

  if(private->circuit.anal_type == MIF_DC){

    /* for DC, initialize values and set the output = output_low */

		t1 = cm_getstore(T1,0);
		t2 = cm_getstore(T2,0);
		t3 = cm_getstore(T3,0);
		t4 = cm_getstore(T4,0);
		set = cm_getstore(SET,0);
		state = cm_getstore(STATE,0);
		output_old = cm_getstore(OUTPUT_OLD,0);

		/* initialize time and state values */
		*t1 = -1;
		*t2 = -1;
		*t3 = -1;
		*t4 = -1;
        *set = 0;
        *state = 0;
        *output_old = output_low;

		private->conn[2]->port[0]->output.rvalue = output_low;
		private->conn[2]->port[0]->partial[0].port[0] = 0; 
		private->conn[2]->port[0]->partial[1].port[0] = 0; 

  }else

  if(private->circuit.anal_type == MIF_TRAN){

/* retrieve previous values, set them equal to the variables 
   Note that these pointer values are immediately dumped into
   other variables because the previous values can't change- 
   can't rewrite the old values */

	t1 = cm_getstore(T1,1);
	t2 = cm_getstore(T2,1);
	t3 = cm_getstore(T3,1);
	t4 = cm_getstore(T4,1);
	set = cm_getstore(SET,1);
	state = cm_getstore(STATE,1);
    clock = cm_getstore(CLOCK,0);
    old_clock = cm_getstore(CLOCK,1);
    output_old = cm_getstore(OUTPUT_OLD,1);

	time1 = *t1;
    time2 = *t2;
    time3 = *t3;
    time4 = *t4;
    set1 = *set;
    state1 = *state;
   
    /* Allocate storage for breakpoint domain & freq. range values */

    x = (double *) calloc(cntl_size, sizeof(double));
    if (x == '\0') {
        cm_send_error_msg(oneshot_allocation_error); 
        return;
    }

    y = (double *) calloc(pw_size, sizeof(double));
    if (y == '\0') {
        cm_send_error_msg(oneshot_allocation_error);  
        return;
    }


    /* Retrieve control and pulse-width values. */       
    for (i=0; i<cntl_size; i++) {
        *(x+i) = private->param[0]->element[i].rvalue;
        *(y+i) = private->param[1]->element[i].rvalue;
    }                       
    

    /* Retrieve cntl_input and clock value. */       
    cntl_input = private->conn[0]->port[0]->input.rvalue;
    *clock = private->conn[1]->port[0]->input.rvalue;


    /* Determine segment boundaries within which cntl_input resides */

    if (cntl_input <= *x) { /* cntl_input below lowest cntl_voltage */
		pw = *y;	
    }
    else 

        /*** cntl_input above highest cntl_voltage ***/
	
        if (cntl_input >= *(x+cntl_size-1)){ 
			pw = *(y+cntl_size-1);

        } else { /*** cntl_input within bounds of end midpoints...   
                must determine position progressively & then 
                calculate required output.                    ***/

            for (i=0; i<cntl_size-1; i++) {

                if ((cntl_input < *(x+i+1)) && (cntl_input >= *(x+i))){ 
            		
					/* Interpolate to get the correct pulse width value */

					pw = ((cntl_input - *(x+i))/(*(x+i+1) - *(x+i)))* 
							(*(y+i+1)-*(y+i)) + *(y+i); 
                } 
    
            }
        }


if(trig_pos_edge){  /* for a positive edge trigger */

    if(!set1){    /* if set1=0, then look for 
                     1.  a rising edge trigger
                     2.  the clock to be higher than the trigger value */

      if((*clock > *old_clock) && (*clock > trig_clk)){

        state1 = 1;
        set1 = 1;

      }

	}else

            /* look for a neg edge before resetting the trigger */

      if((*clock < *old_clock) && (*clock < trig_clk)){

        set1 = 0;
		
      }

}else{
         /* This stuff belongs to the case where a negative edge
			is needed */

    if(!set1){

      if((*clock < *old_clock) && (*clock < trig_clk)){

        state1 = 1;
        set1 = 1;

      }

	}else
            /* look for a pos edge before resetting the trigger */

    if((*clock > *old_clock) && (*clock > trig_clk)){

        set1 = 0;
    }
}

		
    if((state1) && (*output_old - output_low < 1e-20)){

/* if state1 is 1, and the output is low, then set the time points
   and the temporary breakpoints */

   	time1 = private->circuit.time + del_rise; 
   	time2 = time1 + t_rise;
    time3 = time2 + pw + del_fall;
    time4 = time3 + t_fall;

   if((private->circuit.time < time1) || (private->circuit.t[1] == 0)){
    	cm_set_temp_bkpt(time1);
   }

	   cm_set_temp_bkpt(time2);
	   cm_set_temp_bkpt(time3);
	   cm_set_temp_bkpt(time4);

      /* reset the state value */
	   state1 = 0;
	   private->conn[2]->port[0]->output.rvalue = output_low;

   }else{ 

	/* state1 = 1, and the output is high,  then just set time3 and time4 
       and their temporary breakpoints */	

	 if((state1) && (*output_old - output_hi < 1e-20) && (private->param[10]->element[0].bvalue == MIF_TRUE)){

        time3 = private->circuit.time + pw + del_rise + del_fall + t_rise;
        time4 = time3 + t_fall;

        cm_set_temp_bkpt(time3);
        cm_set_temp_bkpt(time4);

        private->conn[2]->port[0]->output.rvalue = output_hi;

        state1 = 0;

      }

	  if(private->param[10]->element[0].bvalue==MIF_FALSE){

         state1 = 0;

      }

   }

   /*  set the value for the output depending on the current time, and
       the values of time1, time2, time3, and time4 */

	if((time1 <= private->circuit.time) && (private->circuit.time <= time2)){

        private->conn[2]->port[0]->output.rvalue = output_low + ((private->circuit.time - time1)/(time2 - time1))*
			(output_hi - output_low); 

	}else
	
	if((time2 <= private->circuit.time) && (private->circuit.time <= time3)){

        private->conn[2]->port[0]->output.rvalue = output_hi;

	}else

	if((time3 <= private->circuit.time) && (private->circuit.time <= time4)){
		
	   private->conn[2]->port[0]->output.rvalue = output_hi + ((private->circuit.time - time3)/(time4 - time3))*
		(output_low - output_hi);

	}else{


  	  private->conn[2]->port[0]->output.rvalue = output_low;

     }

    /* set the variables which need to be stored for the next iteration */

	t1 = cm_getstore(T1,0);
	t2 = cm_getstore(T2,0);
	t3 = cm_getstore(T3,0);
	t4 = cm_getstore(T4,0);
	set = cm_getstore(SET,0);
	state = cm_getstore(STATE,0);
	output_old = cm_getstore(OUTPUT_OLD,0);

	*t1 = time1;
    *t2 = time2;
    *t3 = time3;
    *t4 = time4;
    *set = set1;
    *state = state1;
    *output_old = private->conn[2]->port[0]->output.rvalue;
   

	private->conn[2]->port[0]->partial[0].port[0] = 0;
    private->conn[2]->port[0]->partial[1].port[0] = 0 ;

    } else {                      /* Output AC Gain */

  /* This model has no AC capability */

        ac_gain.real = 0.0; 
        ac_gain.imag= 0.0;
        private->conn[2]->port[0]->ac_gain[0].port[0] = ac_gain;
    }
} 

