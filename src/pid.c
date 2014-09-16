#include "pid.h"

void pid_init(hal_pid_t *pid){
    pid->enable = 1;      /* pin: enable input */
    pid->command = 0;	  /* pin: commanded value */
    pid->commandvds = 0;  /* pin: commanded derivative dummysig */
    pid->commandv = 0;    /* pin: commanded derivative value */
    pid->feedback = 0;    /* pin: feedback value */
    pid->feedbackvds = 0; /* pin: feedback derivative dummysig */
    pid->feedbackv = 0;   /* pin: feedback derivative value */
    pid->error = 0;       /* pin: command - feedback */
    pid->deadband = 0;    /* pin: deadband */
    pid->maxerror = 0;    /* pin: limit for error */
    pid->maxerror_i = 0;  /* pin: limit for integrated error */
    pid->maxerror_d = 0;  /* pin: limit for differentiated error */
    pid->maxcmd_d = 0;    /* pin: limit for differentiated cmd */
    pid->maxcmd_dd = 0;   /* pin: limit for 2nd derivative of cmd */
    pid->error_i = 0;	  /* opt. pin: integrated error */
    pid->prev_error = 0;  /* previous error for differentiator */
    pid->error_d = 0;	  /* opt. pin: differentiated error */
    pid->prev_cmd = 0;    /* previous command for differentiator */
    pid->prev_fb = 0;	  /* previous feedback for differentiator */
    pid->limit_state = 0; /* +1 or -1 if in limit, else 0.0 */
    pid->cmd_d = 0;       /* opt. pin: differentiated command */
    pid->cmd_dd = 0;      /* opt. pin: 2nd derivative of command */
    pid->bias = 0;        /* param: steady state offset */
    pid->pgain = 3;       /* pin: proportional gain */
    pid->igain = 40;       /* pin: integral gain */
    pid->dgain = 0.01;       /* pin: derivative gain */
    pid->ff0gain = 0;	  /* pin: feedforward proportional */
    pid->ff1gain = 0;     /* pin: feedforward derivative */
    pid->ff2gain = 0;     /* pin: feedforward 2nd derivative */
    pid->maxoutput = 1;   /* pin: limit for PID output */
    pid->output = 0;      /* pin: the output value */
    pid->saturated = 0;   /* pin: TRUE when the output is saturated */
    pid->saturated_s = 0; /* pin: the time the output has been saturated */
    pid->saturated_count = 0; /* pin: the time the output has been saturated */
    pid->index_enable = 0;/* pin: to monitor for step changes that would otherwise screw up FF */
    pid->error_previous_target = 0; /* pin: measure error as new position vs previous command, to match motion's ideas */
    pid->prev_ie = 0;
}

void calc_pid(hal_pid_t *arg, float period)
{
    hal_pid_t *pid;
    float tmp1, tmp2, command, feedback;
    int enable;
    float periodfp, periodrecip;

    /* point to the data for this PID loop */
    pid = arg;
    /* precalculate some timing constants */
    periodfp = period * 0.001;
    periodrecip = 1.0 / periodfp;
    /* get the enable bit */
    enable = pid->enable;
    /* read the command and feedback only once */
    command = pid->command;
    feedback = pid->feedback;
    /* calculate the error */
    if((!(pid->prev_ie && !pid->index_enable)) && 
       (pid->error_previous_target)) {
        // the user requests ferror against prev_cmd, and we can honor
        // that request because we haven't just had an index reset that
        // screwed it up.  Otherwise, if we did just have an index
        // reset, we will present an unwanted ferror proportional to
        // velocity for this period, but velocity is usually very small
        // during index search.
        tmp1 = pid->prev_cmd - feedback;
    } else {
        tmp1 = command - feedback;
    }
    /* store error to error pin */
    pid->error = tmp1;
    /* apply error limits */
    if (pid->maxerror != 0.0) {
	if (tmp1 > pid->maxerror) {
	    tmp1 = pid->maxerror;
	} else if (tmp1 < -pid->maxerror) {
	    tmp1 = -pid->maxerror;
	}
    }
    /* apply the deadband */
    if (tmp1 > pid->deadband) {
	tmp1 -= pid->deadband;
    } else if (tmp1 < -pid->deadband) {
	tmp1 += pid->deadband;
    } else {
	tmp1 = 0;
    }
    /* do integrator calcs only if enabled */
    if (enable != 0) {
	/* if output is in limit, don't let integrator wind up */
	if ( ( tmp1 * pid->limit_state ) <= 0.0 ) {
	    /* compute integral term */
	    pid->error_i += tmp1 * periodfp;
	}
	/* apply integrator limits */
	if (pid->maxerror_i != 0.0) {
	    if (pid->error_i > pid->maxerror_i) {
		pid->error_i = pid->maxerror_i;
	    } else if (pid->error_i < -pid->maxerror_i) {
		pid->error_i = -pid->maxerror_i;
	    }
	}
    } else {
	/* not enabled, reset integrator */
	pid->error_i = 0;
    }
    /* compute command and feedback derivatives to dummysigs */
    if(!(pid->prev_ie && !pid->index_enable)) {
        pid->commandvds = (command - pid->prev_cmd) * periodrecip;
        pid->feedbackvds = (feedback - pid->prev_fb) * periodrecip;
    }
    /* and calculate derivative term as difference of derivatives */
    pid->error_d = pid->commandv - pid->feedbackv;
    pid->prev_error = tmp1;
    /* apply derivative limits */
    if (pid->maxerror_d != 0.0) {
	if (pid->error_d > pid->maxerror_d) {
	    pid->error_d = pid->maxerror_d;
	} else if (pid->error_d < -pid->maxerror_d) {
	    pid->error_d = -pid->maxerror_d;
	}
    }
    /* calculate derivative of command */
    /* save old value for 2nd derivative calc later */
    tmp2 = pid->cmd_d;
    if(!(pid->prev_ie && !pid->index_enable)) {
        // not falling edge of index_enable: the normal case
        pid->cmd_d = (command - pid->prev_cmd) * periodrecip;
    }
    // else: leave cmd_d alone and use last period's.  prev_cmd
    // shouldn't be trusted because index homing has caused us to have
    // a step in position.  Using the previous period's derivative is
    // probably a decent approximation since index search is usually a
    // slow steady speed.

    // save ie for next time
    pid->prev_ie = pid->index_enable;

    pid->prev_cmd = command;
    pid->prev_fb = feedback;

    /* apply derivative limits */
    if (pid->maxcmd_d != 0.0) {
	if (pid->cmd_d > pid->maxcmd_d) {
	    pid->cmd_d = pid->maxcmd_d;
	} else if (pid->cmd_d < -pid->maxcmd_d) {
	    pid->cmd_d = -pid->maxcmd_d;
	}
    }
    /* calculate 2nd derivative of command */
    pid->cmd_dd = (pid->cmd_d - tmp2) * periodrecip;
    /* apply 2nd derivative limits */
    if (pid->maxcmd_dd != 0.0) {
	if (pid->cmd_dd > pid->maxcmd_dd) {
	    pid->cmd_dd = pid->maxcmd_dd;
	} else if (pid->cmd_dd < -pid->maxcmd_dd) {
	    pid->cmd_dd = -pid->maxcmd_dd;
	}
    }
    /* do output calcs only if enabled */
    if (enable != 0) {
	/* calculate the output value */
	tmp1 =
	    pid->bias + pid->pgain * tmp1 + pid->igain * pid->error_i +
	    pid->dgain * pid->error_d;
	tmp1 += command * pid->ff0gain + pid->cmd_d * pid->ff1gain +
	    pid->cmd_dd * pid->ff2gain;
	/* apply output limits */
	if (pid->maxoutput != 0.0) {
	    if (tmp1 > pid->maxoutput) {
		tmp1 = pid->maxoutput;
		pid->limit_state = 1.0;
	    } else if (tmp1 < -pid->maxoutput) {
		tmp1 = -pid->maxoutput;
		pid->limit_state = -1.0;
	    } else {
		pid->limit_state = 0.0;
	    }
	}
    } else {
	/* not enabled, force output to zero */
	tmp1 = 0.0;
	pid->limit_state = 0.0;
    }
    /* write final output value to output pin */
    pid->output = tmp1;

    /* set 'saturated' outputs */
    if(pid->limit_state) { 
        pid->saturated = 1;
        pid->saturated_s += period * 1e-3;
        if(pid->saturated_count != 2147483647)
            (pid->saturated_count) ++;
    } else {
        pid->saturated = 0;
        pid->saturated_s = 0;
        pid->saturated_count = 0;
    }
    /* done */
}
