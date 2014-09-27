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
    double ind;
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
    double l;
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
  state.ind = 0.0;
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
      break;
    case mech_spec_s::AC_ASYNC:
      state.volt = 0.0;
      break;
    case mech_spec_s::DC:
      state.volt = state.u - state.v;
      break;
  }

  state.ind = state.vel * elec_spec.v_rps;
  double v = (state.volt - state.ind);
  state.cur += (state.volt - state.ind - state.cur * elec_spec.r) / elec_spec.l * periode;
  //state.cur = (state.volt - state.ind) / elec_spec.r;
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
  double pid_periode;

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

    double friction;
    double load;
    double damping;
    double inertia;

    double p;
    double v;
    double a;
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

  void (*input_cmd)(drive_c* drv, double periode);
  void (*input_feedback)(drive_c* drv, double periode);
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

  est.friction = mot->mech_spec.friction;
  est.load = 0.0;
  est.damping = mot->mech_spec.damping;
  est.inertia = mot->mech_spec.inertia;

  est.pos = 0.0;
  est.vel = 0.0;
  est.acc = 0.0;
  est.cur = 0.0;

  state.mag_pos = 0.0;
  state.mag_vel = 0.0;
  state.ctr = 0.0;
}

void drive_c::step(double periode){
  input_cmd(this, periode);
  input_feedback(this, periode);
  pid(this, periode);
  output(this, periode);
}

void input_cmd(drive_c* drv, double periode){
  double tp, tv;

  tp = drv->cmd.pos;
  tv = drv->cmd.vel;

  switch(drv->in->type){
    case cmd_c::POS:
      drv->cmd.pos = drv->in->get_pos();
      drv->cmd.vel = (drv->cmd.pos - tp) / periode;
      drv->cmd.acc = (drv->cmd.vel - tv) / periode;
      break;
    case cmd_c::POS_VEL:
      drv->cmd.pos = drv->in->get_pos();
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = (drv->cmd.vel - tv) / periode;
      break;
    case cmd_c::POS_VEL_ACC:
      drv->cmd.pos = drv->in->get_pos();
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = drv->in->get_acc();
      break;
    case cmd_c::VEL:
      drv->cmd.pos = drv->cmd.pos + drv->cmd.vel * periode;
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = (drv->cmd.vel - tv) / periode;
      break;
    case cmd_c::VEL_ACC:
      drv->cmd.pos = drv->cmd.pos + drv->cmd.vel * periode;
      drv->cmd.vel = drv->in->get_vel();;
      drv->cmd.acc = drv->in->get_acc();
      break;
  }
}

void input_feedback(drive_c* drv, double periode){
  double t1, t2;
  static double sin_avg_amp = 1.0;
  static double cos_avg_amp = 1.0;
  static double r = 1.0;
  double dpos = 0.0;
  double v = 0.0;
  static double i_sum;
  double p = 0.05;
  double i = 1 * periode;
  double vlo = 0.0 * periode;

  static int point = 0;
  const int hist_size = 10;
  static double pos_hist[hist_size];
  static double vel_hist[hist_size];
  static double acc_hist[hist_size];

  t1 = drv->est.pos;

  switch(drv->mot->feedback.type){
    case mot_c::feedback_s::ENC:
      drv->est.pos = mod(drv->mot->get_enc() * 2 * M_PI / drv->mot->feedback.count - drv->est.offset);
      break;
    case mot_c::feedback_s::RES:
      drv->est.sin_avg = 0.995 * drv->est.sin_avg + 0.005 * drv->mot->get_sin();
      drv->est.cos_avg = 0.995 * drv->est.cos_avg + 0.005 * drv->mot->get_cos();
      // sin_avg_amp = 0.5 * sin_avg_amp + 0.5 * (drv->mot->get_sin() - drv->est.sin_avg) / drv->est.sin_scale * drv->mot->get_polarity();
      // cos_avg_amp = 0.5 * cos_avg_amp + 0.5 * (drv->mot->get_cos() - drv->est.cos_avg) / drv->est.cos_scale * drv->mot->get_polarity();
      //
      // r = 0.99 * r + 0.01 * (sin_avg_amp * sin_avg_amp + cos_avg_amp * cos_avg_amp);
      //
      // if(ABS(sin_avg_amp) < 0.01){
      //   drv->est.cos_scale -= 0.001 * SIGN(1 - cos_avg_amp);
      // }
      //
      // if(ABS(cos_avg_amp) < 0.01){
      //   drv->est.sin_scale -= 0.001 * SIGN(1 - sin_avg_amp);
      // }
      //
      // if(ABS(r - 1.0) > 0.6){
      //   //cerr << "res error r: " << r << endl;
      // }


      dpos = (drv->mot->get_sin() - drv->est.sin_avg) / drv->est.sin_scale * drv->mot->get_polarity() * cos(drv->est.pos + drv->est.offset) - (drv->mot->get_cos() - drv->est.cos_avg) / drv->est.cos_scale * drv->mot->get_polarity() * sin(drv->est.pos + drv->est.offset);
      i_sum += dpos * i + vlo * (drv->state.ctr * drv->dc - drv->est.vel * drv->mot->elec_spec.v_rps) / drv->mot->elec_spec.r * drv->mot->elec_spec.nm_a * periode;
      dpos = p * dpos + i_sum;
      drv->est.pos += dpos;
      drv->est.pos = mod(drv->est.pos);
      pos_hist[point] = drv->est.pos;
      vel_hist[point] = minus_(pos_hist[(point) % hist_size], pos_hist[(point + hist_size - 2) % hist_size]) / (2 * periode);
      acc_hist[point] = minus_(vel_hist[(point) % hist_size], vel_hist[(point + hist_size - 2) % hist_size]) / (2 * periode);
      point++;

      point %= hist_size;
      //drv->est.vel = i_sum / periode;
      drv->est.pos = mod(atan2((drv->mot->get_sin() - drv->est.sin_avg) / drv->est.sin_scale * drv->mot->get_polarity(), (drv->mot->get_cos() - drv->est.cos_avg) / drv->est.cos_scale * drv->mot->get_polarity()) - drv->est.offset);
      break;
    case mot_c::feedback_s::NONE:
    break;
  }

  double a = 0.0;
  v = 0.0;
  p = 0.0;
  double x = 0.0;

  for(int j = 0; j < hist_size; j++){
    a += acc_hist[(point + j) % hist_size];
  }
  a /= hist_size;
  a *= 0.5;

  for(int j = 0; j < hist_size; j++){
    x = (j - hist_size / 2.0) * periode;
    v += vel_hist[(point + j) % hist_size] - (2 * a * x);
  }
  v /= hist_size;

  for(int j = 0; j < hist_size; j++){
    x = (j - hist_size / 2.0) * periode;
    p += pos_hist[(point + j) % hist_size] - (a * x * x + v * x);
  }
  p /= hist_size;

  x = hist_size / 2.0 * periode;
  drv->est.p = (a * x * x + v * x + p);
  drv->est.v = (2 * a * x + v);
  drv->est.a = (2 * a);

  drv->est.pos = drv->mot->state.pos;

  //drv->est.pos = drv->est.p;

  // t2 = drv->est.vel;
  // drv->est.vel = (drv->est.pos - t1) / periode;
  // drv->est.acc = (drv->est.vel - t2) / periode;
}

void input_feedback_real(drive_c* drv, double periode){
  drv->est.pos = drv->mot->state.pos;
  drv->est.vel = drv->mot->state.vel;
  drv->est.acc = drv->mot->state.acc;
}

void pid(drive_c* drv, double periode){
  double p = 180;              // kp
  double i = 15000 * periode;    // ki
  double d = 0.1 / periode; // kd
  double dd = 0.0 / periode;  // rel. vel. kd
  double in = 0.0;            // ind. kp

  double dc_scale = 1.0 / drv->dc; // dc voltage scale
  double ind = drv->mot->state.vel * drv->mot->elec_spec.v_rps / drv->dc;

  double e = minus_(drv->cmd.pos, drv->est.pos);
  static double e_old = 0;
  static double i_sum = 0;

  i_sum += e;
  //i_sum -=
  drv->state.ctr = p * e;
  drv->state.ctr += i * i_sum;
  drv->state.ctr += d * (e - e_old);
  if(abs(e) > 0.01){
    drv->state.ctr += dd * (e - e_old) / e;
  }
  drv->state.ctr *= dc_scale;
  drv->state.ctr += in * ind;

  drv->state.ctr = CLAMP(drv->state.ctr, -1, 1);
  // if(ABS(drv->est.vel) >= drv->mot->mech_spec.max_rps * 0.7){
  //   drv->state.ctr = 0.0;
  // }

  if(abs(drv->state.ctr) >= 0.99){
    i_sum -= e;
  }
  i_sum = CLAMP(i_sum, -1/i, 1/i);

  e_old = e;
}

void pid2(drive_c* drv, double periode){
  double ind = drv->est.vel * drv->mot->elec_spec.v_rps;
  double max_volt_pos = drv->dc * drv->pwm_scale - ind;
  double max_cur_pos = max_volt_pos / drv->mot->elec_spec.r;
  double max_torq_pos = max_cur_pos * drv->mot->elec_spec.nm_a;
  max_torq_pos -= drv->est.friction;
  max_torq_pos -= drv->est.load;
  max_torq_pos -= drv->est.damping * drv->est.vel;
  double max_acc_pos = max_torq_pos / drv->est.inertia;
  double max_dvel_pos = max_acc_pos * periode;

  double max_volt_neg = -drv->dc * drv->pwm_scale - ind;
  double max_cur_neg = max_volt_neg / drv->mot->elec_spec.r;
  double max_torq_neg = max_cur_neg * drv->mot->elec_spec.nm_a;
  max_torq_neg -= drv->est.friction;
  max_torq_neg += drv->est.load;
  max_torq_neg -= drv->est.damping * drv->est.vel;
  double max_acc_neg = max_torq_neg / drv->est.inertia;
  double max_dvel_neg = max_acc_neg * periode;



  drv->state.ctr = 0.0;
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
