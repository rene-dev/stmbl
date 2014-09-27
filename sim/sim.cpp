#include "sim.h"


int main(){
  float sim_time = 0.0;
  float sim_step = 0.00001;
  float sim_end_time = 0.03;

  srand(14235);

  // e240
  mot_c mot;
  mot.reset();
  mot.mech_spec.max_rps = 83.3;
  mot.mech_spec.mot_type = mot_c::mech_spec_s::DC;
  mot.mech_spec.pole_count = 1;
  mot.mech_spec.friction = 0.0;//21;//0.021;
  mot.mech_spec.damping = 0.0000426;//0.0000426;
  mot.mech_spec.inertia = 0.0000268;//0.0000268;

  mot.elec_spec.max_i = 13.9;
  mot.elec_spec.i = 1.9;
  mot.elec_spec.nm_a = 0.135;
  mot.elec_spec.r = 5.4;
  mot.elec_spec.l = 0.0082;
  mot.elec_spec.v_rps = 0.852;
  mot.elec_spec.slip = 0;

  mot.feedback.type = mot_c::feedback_s::RES;
  mot.feedback.count = 1;
  mot.feedback.res_offset = 0.0;
  mot.noise.sin_scale = 1.0;
  mot.noise.cos_scale = 1.0;
  mot.noise.sin_offset = 0.0;
  mot.noise.cos_offset = 0.0;
  mot.noise.var = 0.0;

  mot.load.friction = 0.0;
  mot.load.load = 0.0;
  mot.load.damping = 0.0;
  mot.load.inertia = 0.0;


  cmd_c cmd;
  cmd.reset();
  cmd.periode = 0.5;
  cmd.amplitude = 2;
  cmd.wave = cmd_c::SQUARE;
  cmd.type = cmd_c::POS;
  cmd.pos_res = 0.01;
  cmd.vel_res = 0.01;
  cmd.acc_res = 0.01;

  drive_c drive;
  drive.dc = 5;
  drive.pwm_scale = 0.9;
  drive.pwm_res = 8400;
  drive.pid_periode = 0.001;
  drive.mot = &mot;
  drive.in = &cmd;
  drive.input_cmd = input_cmd;
  drive.input_feedback = input_feedback_real;
  drive.pid = pid;
  drive.output = output;
  drive.reset();
/*

  double ind = 0.0;
  double max_volt_pos = 0.0;
  double max_cur_pos = 0.0;
  double max_torq_pos = 0.0;
  double max_acc_pos = 0.0;
  double max_dvel_pos = 0.0;

  double max_volt_neg = 0.0;
  double max_cur_neg = 0.0;
  double max_torq_neg = 0.0;
  double max_acc_neg = -2;
  double max_dvel_neg = 0.0;

  double vel = 0.0;
  double pos = 0.0;

  int dir = 1;

  double e_pos = 0.0;
  double e_neg = 0.0;
  drive.est.load = 0.0;

  double volt = 0.0;
  double cur = 0.0;
  double torq = 0.0;

  cout << flush << "time vel accp accn e" << endl;

  while(vel >= -mot.mech_spec.max_rps){
    ind = vel * drive.mot->elec_spec.v_rps;

    max_volt_pos = (drive.dc * drive.pwm_scale - drive.dc * (1 - drive.pwm_scale)) - ind;
    max_cur_pos += (max_volt_pos - max_cur_pos * mot.elec_spec.r) / mot.elec_spec.l * sim_step;
    //max_cur_pos = max_volt_pos / drive.mot->elec_spec.r;
    max_torq_pos = max_cur_pos * drive.mot->elec_spec.nm_a;
    max_torq_pos -= drive.est.friction;
    max_torq_pos -= drive.est.load;
    max_torq_pos -= drive.est.damping * vel;
    max_acc_pos = max_torq_pos / drive.est.inertia;

    max_volt_neg = (drive.dc * (1 - drive.pwm_scale) - drive.dc * drive.pwm_scale) - ind;
    max_cur_neg += (max_volt_neg - max_cur_neg * mot.elec_spec.r) / mot.elec_spec.l * sim_step;
    //max_cur_neg = max_volt_neg / drive.mot->elec_spec.r;
    max_torq_neg = max_cur_neg * drive.mot->elec_spec.nm_a;
    max_torq_neg += drive.est.friction;
    max_torq_neg -= drive.est.load;
    max_torq_neg -= drive.est.damping * vel;
    max_acc_neg = max_torq_neg / drive.est.inertia;

    if(dir == 1){
      vel += max_acc_pos * sim_step;
    }
    else {
      vel += max_acc_neg * sim_step;
    }
    pos += vel * sim_step;

    if(vel >= mot.mech_spec.max_rps){
      dir = -1;
    }

    volt = drive.dc * drive.pwm_scale - drive.dc * (1 - drive.pwm_scale);
    cur = volt / drive.mot->elec_spec.r;
    torq = cur * drive.mot->elec_spec.nm_a;
    //e_pos = torq / drive.est.inertia * exp(-sim_time * drive.dc / drive.mot->elec_spec.v_rps );
    e_pos = (torq - drive.est.load) / drive.est.inertia * exp(-sim_time * mot.elec_spec.v_rps / drive.est.inertia * mot.elec_spec.nm_a / mot.elec_spec.r);

    //dp
    //v0
    //v1

    //v = 1/a * maxa * exp(-sim_time * a)
    //e_pos = 30 * torq / drive.est.inertia * (sim_time - 0.15) * (sim_time - 0.15) + 1;

    //e_pos = CLAMP(torq / drive.est.inertia * 0.8 + torq / drive.est.inertia * (-22) * sim_time * 0.5, 0, torq / drive.est.inertia);

    cout << sim_time << ", " << vel << ", " << max_cur_pos << ", " << endl;//", " << max_acc_pos << ", " << max_acc_neg << ", " << e_pos << endl;

    sim_time += sim_step;
  }
  system("gnuplot --persist gp");

  return(0);
*/
  //cout << flush << "time cmd pos est_pos sin_avg sin_scale cos_avg cos_scale" << endl;
  cout << flush << "time curr acc vel" << endl;

  int count = 0;
  int pid_count = drive.pid_periode / sim_step;

  for(sim_time = 0.0; sim_time < sim_end_time; sim_time += sim_step){
    //drive.in->step(sim_step);
    //if(count == 0){
    //  drive.step(sim_step * pid_count);
    //}
    //count++;
    //count %= pid_count;
    drive.state.ctr = 1;
    drive.output(&drive, sim_step);
    drive.mot->step(sim_step);

    //cout << sim_time << ", " << drive.in->get_pos() << ", " << drive.mot->state.pos << ", " << drive.est.pos << ", " << drive.est.sin_avg << ", " << drive.est.sin_scale << ", " << drive.est.cos_avg << ", " << drive.est.cos_scale << endl;
    //cout << sim_time << ", " << drive.mot->state.pos << ", " << drive.mot->state.vel << ", " << drive.mot->state.acc << ", " << drive.est.p << ", " << drive.est.v << ", " << drive.est.a << endl;//", " << drive.state.ctr << ", " << minus_(drive.in->get_pos(), drive.est.pos) << endl;
    //cout << sim_time << ", " << "-15, " << drive.in->get_pos() * 5 - 15<< ", " << drive.state.ctr * 10 << ", " << drive.mot->state.cur << ", " << drive.mot->state.vel / 5 << ", " << drive.mot->state.pos * 5 - 15 << ", " << minus_(drive.in->state.pos, drive.mot->state.pos) * (-100) - 15 << endl;//", " << drive.state.ctr << ", " << minus_(drive.in->get_pos(), drive.est.pos) << endl;
    cout << sim_time << ", " << mot.state.cur << ", " << mot.state.acc / 100 << ", " << mot.state.vel / 10 << endl;
  }

  system("gnuplot --persist gp");

  return(0);
}
