#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

#define ABS(a)	   (((a) < 0) ? -(a) : (a))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define DEG(a) ((a)*M_PI/180.0)
#define RAD(a) ((a)*180.0/M_PI)
#define RAD(a) ((a)*180.0/M_PI)
#define SIGN(a) (((a) < 0) ? (-1) : (((a) > 0) ? (1) : (0)))

double grand(){
  double ret = 0;
  for(int i = 0; i < 12; i++){
    ret += (float)rand() / RAND_MAX;
  }
 return(ret - 6.0);
}

double prand(){
  return(rand() * 2 * M_PI / RAND_MAX - M_PI);
}

// double mod(double pos){
//   return(fmod(pos + M_PI, 2 * M_PI) - M_PI);
// }

double mod(double a){
  while(a < -M_PI){
    a += 2.0 * M_PI;
  }
  while(a > M_PI){
    a -= 2.0 * M_PI;
  }
  return(a);
}

double minus_(double a, double b){
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

class mot_c{
public:
  struct {
    double pos;
    double vel;
    double acc;
    double u;
    double v;
    double w;
    double t;
    double cur;
    double volt;
    double torq;
    int res_polarity;
  } state;

  struct feedback_s{
    enum{
      ENC,
      RES,
      NONE
    } type;
    int count;
    double res_offset;
    int enc_offset;
  } feedback;

  struct {
    double max_i;
    double i;
    double r;
    double nm_a;
    double v_rps;
    double slip;
  } elec_spec;

  struct mech_spec_s{
    double t2; // term. time const
    double max_rps;
    int pole_count;

    enum{
      AC_SYNC,
      AC_ASYNC,
      DC
    } mot_type;

    double friction;
    double damping;
    double inertia; // inertia
  } mech_spec;

  struct{
    double var;
    double sin_offset;
    double sin_scale;
    double cos_offset;
    double cos_scale;
  } noise;

  struct {
    double friction; // friction (torque)
    double load; // asym. torque
    double damping; // damping (torque / vel)
    double inertia; // inertia (torque / acc)
  } load;

  void reset();
  void step(double periode);

  int get_enc();
  double get_sin();
  double get_cos();
  int get_polarity();

  void set_volt(double u, double v, double w);
};

void mot_c::reset(){
  state.pos = 0.0;//prand();
  state.vel = 0.0;
  state.acc = 0.0;
  state.u = 0.0;
  state.v = 0.0;
  state.w = 0.0;
  state.t = 20.0;
  state.cur = 0.0;
  state.volt = 0.0;
  state.res_polarity = 1.0;
  feedback.enc_offset = state.pos;
}

void mot_c::step(double periode){
  double d;
  double q;

  state.res_polarity *= -1;

  switch(mech_spec.mot_type){
    case mech_spec_s::AC_SYNC:
      d = 2/3 * (cos(state.pos * mech_spec.pole_count) * state.u + cos(state.pos * mech_spec.pole_count - 2/3 * M_PI) * state.v + cos(state.pos * mech_spec.pole_count + 2/3 * M_PI) * state.w);
      q = 2/3 * (-sin(state.pos * mech_spec.pole_count) * state.u - sin(state.pos * mech_spec.pole_count - 2/3 * M_PI) * state.v - sin(state.pos * mech_spec.pole_count + 2/3 * M_PI) * state.w);
      state.volt = q;
      state.cur = (q - state.vel * elec_spec.v_rps) / elec_spec.r;
      break;
    case mech_spec_s::AC_ASYNC:
      state.cur = 0.0;
      break;
    case mech_spec_s::DC:
      state.volt = state.u - state.v;
      state.cur = (state.u - state.v - state.vel * elec_spec.v_rps) / elec_spec.r;
      break;
  }

  state.torq = state.cur * elec_spec.nm_a - (mech_spec.damping + load.damping) * state.vel + load.load;
  if(abs(state.torq) < mech_spec.friction + load.friction){
    state.torq = 0.0;
  }
  else{
    state.torq = state.torq - SIGN(state.torq) * (mech_spec.friction + load.friction);
  }

  state.acc = state.torq / (mech_spec.inertia + load.inertia);
  state.vel += state.acc * periode;
  state.pos += state.vel * periode;
  state.pos = mod(state.pos);
  if(state.cur  > elec_spec.max_i){
    cerr << "MOT: cur > max_i: " << state.cur << endl;
  }
  if(state.vel  > mech_spec.max_rps){
    cerr << "MOT: vel > max_v: " << state.vel << endl;
  }
}

void mot_c::set_volt(double u, double v, double w){
  state.u = u;
  state.v = v;
  state.w = w;
}

int mot_c::get_enc(){
  return((int)((state.pos - feedback.enc_offset + 2 * M_PI) * feedback.count / 2 / M_PI) % feedback.count);
}

double mot_c::get_sin(){
  return(sin((state.pos + feedback.res_offset) * feedback.count) * noise.sin_scale * state.res_polarity + noise.sin_offset + grand() * noise.var);
}

double mot_c::get_cos(){
  return(cos((state.pos + feedback.res_offset) * feedback.count) * noise.cos_scale * state.res_polarity + noise.cos_offset + grand() * noise.var);
}

int mot_c::get_polarity(){
  return(state.res_polarity);
}

class cmd_c{
public:
  struct {
    double time;
    double pos;
    double vel;
    double acc;
  } state;

  enum{
    POS,
    POS_VEL,
    POS_VEL_ACC,
    VEL,
    VEL_ACC
  } type;

  enum{
    SINE,
    SQUARE,
    RAMP,
    CONST
  } wave;

  double periode;
  double amplitude;
  double max_vel;
  double max_acc;

  double pos_res;
  double vel_res;
  double acc_res;

  void reset();
  void step(double periode);

  double get_pos();
  double get_vel();
  double get_acc();
};

void cmd_c::reset(){
  state.time = 0.0;
  state.pos = 0.0;
  state.vel = 0.0;
  state.acc = 0.0;
}

void cmd_c::step(double s){
  state.time += s;

  switch(wave){
    case SINE:
      state.pos = mod(sin(state.time / periode * 2 * M_PI) * amplitude);
      state.vel = cos(state.time / periode * 2 * M_PI) * amplitude;
      state.acc = -sin(state.time / periode * 2 * M_PI) * amplitude;
      break;
    case SQUARE:
      state.pos = mod(((int)(state.time / periode * 2) % 2) * amplitude);
      state.vel = 0;
      state.acc = 0;
      break;
    case RAMP:
      state.pos = mod(state.time * amplitude);
      state.vel = 0;
      state.acc = 0;
      break;
    case CONST:
      state.pos = mod(amplitude);
      state.vel = 0;
      state.acc = 0;
      break;
  }
}

double cmd_c::get_pos(){
  return((int)(state.pos / pos_res) * pos_res);
}

double cmd_c::get_vel(){
  return((int)(state.vel / vel_res) * vel_res);
}

double cmd_c::get_acc(){
  return((int)(state.acc / acc_res) * acc_res);
}


class drive_c{
public:
  struct cmd_s{
    double pos;
    double vel;
    double acc;
  } cmd;

  double dc;
  double pwm_scale;
  int pwm_res;

  struct {
    double offset;
    double sin_scale;
    double sin_avg;
    double cos_scale;
    double cos_avg;
    double res_var;

    double pos;
    double vel;
    double acc;
    double cur;
  } est;

  struct {
    double mag_pos;
    double mag_vel;
    double ctr;
  } state;

  mot_c* mot;
  cmd_c* in;

  void reset();
  void step(double periode);

  void (*input)(drive_c* drv, double periode);
  void (*pid)(drive_c* drv, double periode);
  void (*output)(drive_c* drv, double periode);

  void set_pos(double pos);
  void set_pos_vel(double pos, double vel);
  void set_pos_vel_acc(double pos, double vel, double acc);
  void set_vel(double pos);
  void set_vel_acc(double pos, double acc);
};

void drive_c::reset(){
  cmd.pos = 0.0;
  cmd.vel = 0.0;
  cmd.acc = 0.0;

  est.offset = 0.0;
  est.sin_scale = 1.0;
  est.cos_scale = 1.0;
  est.sin_avg = 0.0;
  est.cos_avg = 0.0;
  est.res_var = 0.0;

  est.pos = 0.0;
  est.vel = 0.0;
  est.acc = 0.0;
  est.cur = 0.0;

  state.mag_pos = 0.0;
  state.mag_vel = 0.0;
  state.ctr = 0.0;
}

void drive_c::step(double periode){
  input(this, periode);
  pid(this, periode);
  output(this, periode);
}

void input(drive_c* drv, double periode){
  double t1, t2;
  static double sin_avg_amp = 1.0;
  static double cos_avg_amp = 1.0;
  static double r = 1.0;

  t1 = drv->est.pos;

  switch(drv->mot->feedback.type){
    case mot_c::feedback_s::ENC:
      drv->est.pos = mod(drv->mot->get_enc() * 2 * M_PI / drv->mot->feedback.count - drv->est.offset);
      break;
    case mot_c::feedback_s::RES:
      drv->est.sin_avg = 0.995 * drv->est.sin_avg + 0.005 * drv->mot->get_sin();
      drv->est.cos_avg = 0.995 * drv->est.cos_avg + 0.005 * drv->mot->get_cos();
      sin_avg_amp = 0.5 * sin_avg_amp + 0.5 * (drv->mot->get_sin() - drv->est.sin_avg) / drv->est.sin_scale * drv->mot->get_polarity();
      cos_avg_amp = 0.5 * cos_avg_amp + 0.5 * (drv->mot->get_cos() - drv->est.cos_avg) / drv->est.cos_scale * drv->mot->get_polarity();

      r = 0.99 * r + 0.01 * (sin_avg_amp * sin_avg_amp + cos_avg_amp * cos_avg_amp);

      if(ABS(sin_avg_amp) < 0.01){
        //drv->est.cos_scale -= 0.001 * SIGN(1 - cos_avg_amp);
      }

      if(ABS(cos_avg_amp) < 0.01){
        //drv->est.sin_scale -= 0.001 * SIGN(1 - sin_avg_amp);
      }

      if(ABS(r - 1.0) > 0.6){
        //cerr << "res error r: " << r << endl;
      }

      drv->est.pos = mod(atan2((drv->mot->get_sin() - drv->est.sin_avg) / drv->est.sin_scale * drv->mot->get_polarity(), (drv->mot->get_cos() - drv->est.cos_avg) / drv->est.cos_scale * drv->mot->get_polarity()) - drv->est.offset);
      break;
    case mot_c::feedback_s::NONE:
    break;
  }

  t2 = drv->est.vel;
  drv->est.vel = drv->est.pos - t1;
  drv->est.acc = drv->est.vel - t2;

  switch(drv->in->type){
    case cmd_c::POS:
      drv->cmd.pos = drv->in->get_pos();
      drv->cmd.vel = 0.0;
      drv->cmd.acc = 0.0;
      break;
    case cmd_c::POS_VEL:
      drv->cmd.pos = drv->in->get_pos();
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = 0.0;
      break;
    case cmd_c::POS_VEL_ACC:
      drv->cmd.pos = drv->in->get_pos();
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = drv->in->get_acc();
      break;
    case cmd_c::VEL:
      drv->cmd.pos = 0.0;
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = 0.0;
      break;
    case cmd_c::VEL_ACC:
      drv->cmd.pos = 0.0;
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = drv->in->get_acc();
      break;
  }
}

void pid(drive_c* drv, double periode){
  double p = 4;
  double i = 40 * periode;
  double d = 0.001 / periode;

  double e = minus_(drv->cmd.pos, drv->est.pos);
  static double e_old = 0;
  static double i_sum = 0;

  i_sum += e;

  drv->state.ctr = CLAMP(p * e + i * i_sum + d * (e - e_old), -1, 1);

  if(abs(drv->state.ctr) >= 0.99){
    i_sum -= e;
  }

  e_old = e;
}

void output(drive_c* drv, double periode){
  double u, v, w;

  switch(drv->mot->mech_spec.mot_type){
    case mot_c::mech_spec_s::AC_SYNC:
    case mot_c::mech_spec_s::AC_ASYNC:
    case mot_c::mech_spec_s::DC:
      u = ((int)((drv->state.ctr * drv->pwm_scale + 1) / 2.0 * drv->pwm_res)) * drv->dc / drv->pwm_res;
      v = ((int)((drv->state.ctr * drv->pwm_scale * (-1) + 1) / 2.0  * drv->pwm_res)) * drv->dc / drv->pwm_res;
      w = 0;
    break;
  }

  drv->mot->set_volt(u, v, w);
}
