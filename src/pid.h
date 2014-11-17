#pragma once
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ABS(a)	   (((a) < 0) ? -(a) : (a))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define LIMIT(x, lowhigh)  (((lowhigh) > 0.0) ? (((x) > (lowhigh)) ? (lowhigh) : (((x) < (-lowhigh)) ? (-lowhigh) : (x))) : (x))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define DEG(a) ((a)*M_PI/180.0)
#define RAD(a) ((a)*180.0/M_PI)
#define SIGN(a) (((a) < 0) ? (-1) : (((a) > 0) ? (1) : (0)))

typedef struct {
    int enable;        /* pin: enable input */
    //float command;	   /* pin: commanded value */
    //float commandvds;  /* pin: commanded derivative dummysig */
    float commandv;    /* pin: commanded derivative value */
    float commanda;    /* pin: commandedv derivative value */
    //float feedback;    /* pin: feedback value */
    //float feedbackvds; /* pin: feedback derivative dummysig */
    float feedbackv;   /* pin: feedback derivative value */
    float feedbacka;   /* pin: feedbackv derivative value */
    float error;       /* pin: command - feedback */
    float deadband;    /* pin: deadband */
    float maxerror;    /* pin: limit for error */
    float maxerror_i;  /* pin: limit for integrated error */
    float maxerror_d;  /* pin: limit for differentiated error */
    float maxerror_dd;  /* pin: limit for differentiated error */
    float maxcmd_d;    /* pin: limit for differentiated cmd */
    float maxcmd_dd;   /* pin: limit for 2nd derivative of cmd */
    float error_i;	   /* opt. pin: integrated error */
    float prev_error;  /* previous error for differentiator */
    float error_d;	   /* opt. pin: differentiated error */
    float error_dd;	   /* opt. pin: differentiated error */
    //float prev_cmd;    /* previous command for differentiator */
    //float prev_fb;	   /* previous feedback for differentiator */
    float limit_state; /* +1 or -1 if in limit, else 0.0 */
    float cmd_d;       /* opt. pin: differentiated command */
    float cmd_dd;      /* opt. pin: 2nd derivative of command */
    float bias;        /* param: steady state offset */
    float pgain;       /* pin: proportional gain */
    float igain;       /* pin: integral gain */
    float dgain;       /* pin: derivative gain */
    float bgain;       /* pin: derivative gain */
    // float ff0gain;	   /* pin: feedforward proportional */
    float ff1gain;     /* pin: feedforward derivative */
    float ff2gain;     /* pin: feedforward 2nd derivative */
    float maxoutput;   /* pin: limit for PID output */
    float output;      /* pin: the output value */
    int   saturated;   /* pin: TRUE when the output is saturated */
    float saturated_s; /* pin: the time the output has been saturated */
    int   saturated_count; /* pin: the time the output has been saturated */
    //int index_enable;  /* pin: to monitor for step changes that would otherwise screw up FF */
    //int error_previous_target; /* pin: measure error as new position vs previous command, to match motion's ideas */
    //char prev_ie;
} hal_pid_t;

typedef struct {
  // pos
  float ext_cmd_pos;
  float feedback_pos;

  // vel
  float cmd_vel;
  float ext_cmd_vel;
  float feedback_vel;

  // acc
  float cmd_acc;
  float ext_cmd_acc;
  //float feedback_acc;

  // force
  float cmd_force;
  float ext_cmd_force;
  //float feedback_force;

  // current
  float cmd_cur;
  float feedback_cur;

  // volt
  float cmd_volt;

  // pwm output
  float cmd_pwm;


  // error
  float pos_error;
  float vel_error;
  float cur_error;


  // parameters
  float enable;

  // pos -> vel
  float pos_p;
  float ff1;

  // vel -> acc
  float vel_p;
  float vel_i;
  float ff2;

  // acc -> force
  float acc_p;

  // force -> current
  float force_p;

  // current -> volt
  float cur_p;
  float cur_d;
  float ind_p;

  // voltage -> pwm
  float volt;

  // time
  float period;

  // limits
  float max_vel;
  float max_vel_error_sum;

  float max_acc;

  float max_force;

  float max_cur;

  float max_volt;

  float max_pwm;

  float i0;


  // state
  float vel_error_sum;

  float cmd_cur_old;

  float saturated_s;

  float i2t;

  // operator
  // pos_error = minus(cmd_pos, feedback_pos)
  float (*minus)(float a, float b);
}pid2p;

void pid2(pid2p* pid);
void pid2_init(pid2p* pid);

void calc_pid(hal_pid_t *arg, float period);
void pid_init(hal_pid_t *pid);
float minus(float a, float b);
float mod(float a);

#ifdef __cplusplus
}
#endif
