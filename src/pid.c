#include "pid.h"

float minus(float a, float b){
	if(ABS(a - b) < M_PI){
		return(a - b);
	}
	else if(a > b){
		return(a - b - 2.0 * M_PI);
	}
	else{
		return(a - b + 2.0 * M_PI);
	}
}

float mod(float a){
	while(a < -M_PI){
		a += 2.0 * M_PI;
	}
	while(a > M_PI){
		a -= 2.0 * M_PI;
	}
	return(a);
}

void pid_init(hal_pid_t *pid){
    pid->enable = 0;      /* pin: enable input */
    //pid->command = 0;	  /* pin: commanded value */
    //pid->commandvds = 0;  /* pin: commanded derivative dummysig */
    pid->commandv = 0;    /* pin: commanded derivative value */
	pid->commanda = 0;    /* pin: commandedv derivative value */
    //pid->feedback = 0;    /* pin: feedback value */
    //pid->feedbackvds = 0; /* pin: feedback derivative dummysig */
    pid->feedbackv = 0;   /* pin: feedback derivative value */
	pid->feedbacka = 0;   /* pin: feedbackv derivative value */
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
    pid->error_dd = 0;	  /* opt. pin: differentiated error */
    //pid->prev_cmd = 0;    /* previous command for differentiator */
    //pid->prev_fb = 0;	  /* previous feedback for differentiator */
    pid->limit_state = 0; /* +1 or -1 if in limit, else 0.0 */
    pid->cmd_d = 0;       /* opt. pin: differentiated command */
    pid->cmd_dd = 0;      /* opt. pin: 2nd derivative of command */
    pid->bias = 0;        /* param: steady state offset */
    pid->pgain = 2;       /* pin: proportional gain */
    pid->igain = 0.04;       /* pin: integral gain */
    pid->dgain = 0.015;       /* pin: derivative gain */
    pid->bgain = 0.0;       /* pin: derivative gain */
        // pid->ff0gain = 0;      /* pin: feedforward proportional */
    pid->ff1gain = 0.005;     /* pin: feedforward derivative */
    pid->ff2gain = 0.00005;     /* pin: feedforward 2nd derivative */
    pid->maxoutput = 1;   /* pin: limit for PID output */
    pid->output = 0;      /* pin: the output value */
    pid->saturated = 0;   /* pin: TRUE when the output is saturated */
    pid->saturated_s = 0; /* pin: the time the output has been saturated */
    pid->saturated_count = 0; /* pin: the time the output has been saturated */
    //pid->index_enable = 0;/* pin: to monitor for step changes that would otherwise screw up FF */
    //pid->error_previous_target = 0; /* pin: measure error as new position vs previous command, to match motion's ideas */
    //pid->prev_ie = 0;
}

void calc_pid(hal_pid_t *arg, float period)
{
    hal_pid_t *pid;
    float tmp1;
    int enable;

    /* point to the data for this PID loop */
    pid = arg;
    /* precalculate some timing constants */
    /* get the enable bit */
    enable = pid->enable;
    /* read the command and feedback only once */
    //command = pid->command;
    //feedback = pid->feedback;


    // /* calculate the error */
    // if((!(pid->prev_ie && !pid->index_enable)) &&
    //    (pid->error_previous_target)) {
    //     // the user requests ferror against prev_cmd, and we can honor
    //     // that request because we haven't just had an index reset that
    //     // screwed it up.  Otherwise, if we did just have an index
    //     // reset, we will present an unwanted ferror proportional to
    //     // velocity for this period, but velocity is usually very small
    //     // during index search.
    //     tmp1 = pid->prev_cmd - feedback;
    // } else {
    //     tmp1 = command - feedback;
    // }
    // /* store error to error pin */
    // pid->error = tmp1;
    tmp1 = pid->error;

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
    if (enable != 0 && pid->igain != 0) {
	/* if output is in limit, don't let integrator wind up */
	if ( ( tmp1 * pid->limit_state ) <= 0.0 ) {
	    /* compute integral term */
	    pid->error_i += tmp1 * period;
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
    // if(!(pid->prev_ie && !pid->index_enable)) {
    //     pid->commandvds = (command - pid->prev_cmd) * periodrecip;
    //     pid->feedbackvds = (feedback - pid->prev_fb) * periodrecip;
    // }
    /* and calculate derivative term as difference of derivatives */
    //tmp2 = pid->error_d;
    pid->error_d = pid->commandv - pid->feedbackv;
    //pid->prev_error = tmp1;
    /* apply derivative limits */
    if (pid->maxerror_d != 0.0) {
	if (pid->error_d > pid->maxerror_d) {
	    pid->error_d = pid->maxerror_d;
	} else if (pid->error_d < -pid->maxerror_d) {
	    pid->error_d = -pid->maxerror_d;
	}
    }

    //pid->error_dd = (pid->error_d - tmp2) * periodrecip;
    pid->error_dd = pid->commanda - pid->feedbacka;
		/* apply 2nd derivative limits */
    if (pid->maxerror_dd != 0.0) {
	if (pid->error_dd > pid->maxerror_dd) {
	    pid->error_dd = pid->maxerror_dd;
	} else if (pid->error_dd < -pid->maxerror_dd) {
	    pid->error_dd = -pid->maxerror_dd;
	}
    }
    /* calculate derivative of command */
    /* save old value for 2nd derivative calc later */
    //tmp2 = pid->cmd_d;
    //if(!(pid->prev_ie && !pid->index_enable)) {
        // not falling edge of index_enable: the normal case
    pid->cmd_d = pid->commandv;
    //}
    // else: leave cmd_d alone and use last period's.  prev_cmd
    // shouldn't be trusted because index homing has caused us to have
    // a step in position.  Using the previous period's derivative is
    // probably a decent approximation since index search is usually a
    // slow steady speed.

    // save ie for next time
    // pid->prev_ie = pid->index_enable;

    // pid->prev_cmd = command;
    // pid->prev_fb = feedback;

    /* apply derivative limits */
    if (pid->maxcmd_d != 0.0) {
	if (pid->cmd_d > pid->maxcmd_d) {
	    pid->cmd_d = pid->maxcmd_d;
	} else if (pid->cmd_d < -pid->maxcmd_d) {
	    pid->cmd_d = -pid->maxcmd_d;
	}
    }
    /* calculate 2nd derivative of command */
    //pid->cmd_dd = (pid->cmd_d - tmp2) * periodrecip * 0.5 + pid->cmd_dd * 0.5;
    pid->cmd_dd = pid->commanda;
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
	    pid->dgain * pid->error_d + pid->bgain * pid->error_dd;
	tmp1 += /*command * pid->ff0gain + */ pid->cmd_d * pid->ff1gain +
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
        pid->saturated_s += period;
        if(pid->saturated_count != 2147483647)
            (pid->saturated_count) ++;
    } else {
        pid->saturated = 0;
        pid->saturated = 0;
        pid->saturated_count = 0;
    }
    /* done */
}



void pid2(pid2p* pid){
	if(pid->enable > 0.5){
		// pos -> vel
		pid->pos_error = minus(pid->ext_cmd_pos, pid->feedback_pos);
		pid->cmd_vel = CLAMP(pid->pos_p * pid->pos_error + pid->ff1 * pid->ext_cmd_vel, -pid->max_vel, pid->max_vel);

		// vel -> acc
		pid->vel_error = pid->cmd_vel - pid->feedback_vel;
		if(pid->vel_i > 0.0){
			pid->vel_error_sum = CLAMP(pid->vel_error_sum + pid->vel_error * pid->period, -pid->max_vel_error_sum, pid->max_vel_error_sum);
		}
		else{
			pid->vel_error_sum = 0.0;
		}
		pid->cmd_acc = CLAMP(pid->vel_p * pid->vel_error + pid->vel_i * pid->vel_error_sum + pid->ff2 * pid->ext_cmd_acc, -pid->max_acc, pid->max_acc);

		// acc -> force
		pid->cmd_force = CLAMP(pid->acc_p * pid->cmd_acc + pid->ext_cmd_force, -pid->max_force, pid->max_force);

		// force -> current
		pid->cmd_cur = CLAMP(pid->force_p * pid->cmd_force, -pid->max_cur, pid->max_cur);

		// current -> volt
		pid->cur_error = pid->cmd_cur - pid->feedback_cur;
		pid->cmd_volt = CLAMP(pid->cur_p * pid->cur_error + pid->cur_d * (pid->cmd_cur - pid->cmd_cur_old) / pid->period + pid->ind_p * pid->feedback_vel, -pid->max_volt, pid->max_volt);
		pid->cmd_cur_old = pid->cmd_cur;

		// volt -> pwm
		pid->cmd_pwm = pid->cmd_volt / pid->volt;
		if(pid->cmd_pwm >= pid->max_pwm || pid->cmd_pwm <= -pid->max_pwm){
			pid->saturated_s += pid->period;
		}
		else{
			pid->saturated_s = 0.0;
		}
		pid->cmd_pwm = CLAMP(pid->cmd_pwm, -pid->max_pwm, pid->max_pwm);
	}
	else{
		pid->cmd_vel = 0.0;
		pid->cmd_acc = 0.0;
		pid->cmd_force = 0.0;
		pid->cmd_cur = 0.0;
		pid->cmd_volt = 0.0;
		pid->cmd_vel = 0.0;

		pid->vel_error_sum = 0.0;

		pid->cmd_cur_old = 0.0;

		pid->saturated_s = 0.0;
	}
}


void pid2_init(pid2p* pid){
	pid->ext_cmd_pos = 0.0;
	pid->feedback_pos = 0.0;

	pid->cmd_vel = 0.0;
	pid->ext_cmd_vel = 0.0;
	pid->feedback_vel = 0.0;

	pid->cmd_acc = 0.0;
	pid->ext_cmd_acc = 0.0;
	//pid->feedback_acc = 0.0;

	pid->cmd_force = 0.0;
	pid->ext_cmd_force = 0.0;
	//pid->feedback_force = 0.0;

	pid->cmd_cur = 0.0;
	pid->feedback_cur = 0.0;

	pid->cmd_volt = 0.0;

	pid->cmd_pwm = 0.0;


	pid->pos_error = 0.0;
	pid->vel_error = 0.0;
	pid->cur_error = 0.0;

	pid->enable = 1.0;

	pid->pos_p = 30.0;
	pid->ff1 = 0.95;

	pid->vel_p = 1.0;
	pid->vel_i = 40.0;
	pid->ff2 = 0.002;

	pid->acc_p = 0.1;

	pid->force_p = 3.667;

	pid->cur_p = 15.0;
	pid->cur_d = 0.01;
	pid->ind_p = 0.57;

	pid->volt = 130.0;

	pid->period = 0.001;


	pid->max_vel = 62.9;
	pid->max_vel_error_sum = 2.5;

	pid->max_acc = 1200;

	pid->max_force = 100.0;

	pid->max_cur = 6.0;

	pid->max_volt = 130.0;

	pid->max_pwm = 0.9;


	pid->vel_error_sum = 0.0;

	pid->cmd_cur_old = 0.0;

	pid->saturated_s = 0.0;

	pid->minus = minus;
}
