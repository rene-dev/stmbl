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

float grand(){
  float ret = 0;
  for(int i = 0; i < 12; i++){
    ret += (float)rand() / RAND_MAX;
  }
 return(ret - 6.0);
}

float prand(){
  return(rand() * 2 * M_PI / RAND_MAX - M_PI);
}

// float mod(float pos){
//   return(fmod(pos + M_PI, 2 * M_PI) - M_PI);
// }

float mod(float a){
  while(a < -M_PI){
    a += 2.0 * M_PI;
  }
  while(a > M_PI){
    a -= 2.0 * M_PI;
  }
  return(a);
}

float minus_(float a, float b){
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
    float pos;
    float vel;
    float acc;
    float u;
    float v;
    float w;
    float t;
    float cur;
    float volt;
    float torq;
    float res_polarity;
  } state;

  struct feedback_s{
    enum{
      ENC,
      RES,
      NONE
    } type;
    int count;
    float res_offset;
    int enc_offset;
  } feedback;

  struct {
    float max_i;
    float i;
    float r;
    float nm_a;
    float v_rps;
    float slip;
  } elec_spec;

  struct mech_spec_s{
    float t2; // term. time const
    float max_rps;
    int pole_count;

    enum{
      AC_SYNC,
      AC_ASYNC,
      DC
    } mot_type;

    float friction;
    float damping;
    float inertia; // inertia
  } mech_spec;

  struct{
    float var;
    float sin_offset;
    float sin_scale;
    float cos_offset;
    float cos_scale;
  } noise;

  struct {
    float friction; // friction (torque)
    float load; // asym. torque
    float damping; // damping (torque / vel)
    float inertia; // inertia (torque / acc)
  } load;

  void reset();
  void step(float periode);

  int get_enc();
  float get_sin();
  float get_cos();

  void set_volt(float u, float v, float w);
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

void mot_c::step(float periode){
  float d;
  float q;

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

  state.torq = state.cur * elec_spec.nm_a - mech_spec.damping * state.vel - load.damping * state.vel + load.load;
  if(abs(state.torq) < mech_spec.friction + load.friction){
    state.torq = 0.0;
  }

  state.acc = state.torq / (mech_spec.inertia + load.inertia);
  state.vel += state.acc * periode;
  state.pos += state.vel * periode;
  state.pos = mod(state.pos);
  if(state.cur  > elec_spec.max_i){
    cout << "MOT: cur > max_i: " << state.cur << endl;
  }
  if(state.vel  > mech_spec.max_rps){
    cout << "MOT: vel > max_v: " << state.vel << endl;
  }
}

void mot_c::set_volt(float u, float v, float w){
  state.u = u;
  state.v = v;
  state.w = w;
}

int mot_c::get_enc(){
  return((int)((state.pos - feedback.enc_offset + 2 * M_PI) * feedback.count / 2 / M_PI) % feedback.count);
}

float mot_c::get_sin(){
  return(sin(state.pos * feedback.count) * noise.sin_scale * state.res_polarity + noise.sin_offset + grand() * noise.var);
}

float mot_c::get_cos(){
  return(cos(state.pos * feedback.count) * noise.cos_scale * state.res_polarity + noise.cos_offset + grand() * noise.var);
}


class cmd_c{
public:
  struct {
    float time;
    float pos;
    float vel;
    float acc;
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
    //TRAPEZOID,
    CONST
  } wave;

  float periode;
  float amplitude;
  float max_vel;
  float max_acc;

  float pos_res;
  float vel_res;
  float acc_res;

  void reset();
  void step(float periode);

  float get_pos();
  float get_vel();
  float get_acc();
};

void cmd_c::reset(){
  state.time = 0.0;
  state.pos = 0.0;
  state.vel = 0.0;
  state.acc = 0.0;
}

void cmd_c::step(float s){
  state.time += s;

  switch(wave){
    case SINE:
      state.pos = sin(state.time / periode * 2 * M_PI) * amplitude;
      state.vel = cos(state.time / periode * 2 * M_PI) * amplitude;
      state.acc = -sin(state.time / periode * 2 * M_PI) * amplitude;
      break;
    case SQUARE:
      state.pos = ((int)(state.time / periode * 2) % 2) * amplitude;
      state.vel = 0;
      state.acc = 0;
      break;
    case CONST:
      state.pos = amplitude;
      state.vel = 0;
      state.acc = 0;
      break;
  }
}

float cmd_c::get_pos(){
  return((int)(state.pos / pos_res) * pos_res);
}

float cmd_c::get_vel(){
  return((int)(state.vel / vel_res) * vel_res);
}

float cmd_c::get_acc(){
  return((int)(state.acc / acc_res) * acc_res);
}


class drive_c{
public:
  struct cmd_s{
    float pos;
    float vel;
    float acc;
  } cmd;

  float dc;
  float pwm_scale;
  int pwm_res;

  struct {
    float offset;
    float sin_scale;
    float sin_avg;
    float cos_scale;
    float cos_avg;
    float res_var;

    float pos;
    float vel;
    float acc;
    float cur;
  } est;

  struct {
    float mag_pos;
    float mag_vel;
    float ctr;
  } state;

  mot_c* mot;
  cmd_c* in;

  void reset();
  void step(float periode);

  void input(float periode);
  void pid(float periode);
  void output(float periode);

  void set_pos(float pos);
  void set_pos_vel(float pos, float vel);
  void set_pos_vel_acc(float pos, float vel, float acc);
  void set_vel(float pos);
  void set_vel_acc(float pos, float acc);
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

void drive_c::step(float periode){
  input(periode);
  pid(periode);
  output(periode);
}

void drive_c::input(float periode){
  float t1, t2;
  t1 = est.pos;

  switch(mot->feedback.type){
    case mot_c::feedback_s::ENC:
      est.pos = mod(mot->get_enc() * 2 * M_PI / mot->feedback.count - est.offset);
      break;
    case mot_c::feedback_s::RES:
      est.pos = mod(atan2((mot->get_sin() - est.sin_avg) * est.sin_scale, (mot->get_cos() - est.cos_avg) * est.cos_avg) - est.offset);
      break;
    case mot_c::feedback_s::NONE:
    break;
  }

  t2 = est.vel;
  est.vel = est.pos - t1;
  est.acc = est.vel - t2;

  switch(in->type){
    case cmd_c::POS:
      cmd.pos = in->get_pos();
      cmd.vel = 0.0;
      cmd.acc = 0.0;
      break;
    case cmd_c::POS_VEL:
      cmd.pos = in->get_pos();
      cmd.vel = in->get_vel();;
      cmd.acc = 0.0;
      break;
    case cmd_c::POS_VEL_ACC:
      cmd.pos = in->get_pos();
      cmd.vel = in->get_vel();;
      cmd.acc = in->get_acc();
      break;
    case cmd_c::VEL:
      cmd.pos = 0.0;
      cmd.vel = in->get_vel();;
      cmd.acc = 0.0;
      break;
    case cmd_c::VEL_ACC:
      cmd.pos = 0.0;
      cmd.vel = in->get_vel();;
      cmd.acc = in->get_acc();
      break;
  }
}

void drive_c::pid(float periode){
  float p = 4;
  float i = 40 * periode;
  float d = 0.001 / periode;

  float e = minus_(cmd.pos, est.pos);
  static float e_old = 0;
  static float i_sum = 0;

  i_sum += e;

  state.ctr = CLAMP(p * minus_(cmd.pos, est.pos) + i * i_sum + d * (e - e_old), -1, 1);

  if(abs(state.ctr) >= 0.99){
    i_sum -= e;
  }

  e_old = e;
}

void drive_c::output(float periode){
  float u, v, w;

  switch(mot->mech_spec.mot_type){
    case mot_c::mech_spec_s::AC_SYNC:
    case mot_c::mech_spec_s::AC_ASYNC:
    case mot_c::mech_spec_s::DC:
      u = ((int)((state.ctr * pwm_scale + 1) / 2.0 * pwm_res)) * dc / pwm_res;
      v = ((int)((state.ctr * pwm_scale * (-1) + 1) / 2.0  * pwm_res)) * dc / pwm_res;
      w = 0;
    break;
  }

  mot->set_volt(u, v, w);
}
