#include "sim.h"


int main(){
  float sim_time = 0;
  float sim_step = 0.001;
  float sim_end_time = 5.0;

  srand(14235);

  // e240
  mot_c mot;
  mot.reset();
  mot.mech_spec.max_rps = 83.3;
  mot.mech_spec.mot_type = mot_c::mech_spec_s::DC;
  mot.mech_spec.pole_count = 1;
  mot.mech_spec.friction = 0.021;
  mot.mech_spec.damping = 0.0118;
  mot.mech_spec.inertia = 0.0000268;

  mot.elec_spec.max_i = 13.9;
  mot.elec_spec.i = 1.9;
  mot.elec_spec.nm_a = 0.135;
  mot.elec_spec.r = 5.4;
  mot.elec_spec.v_rps = 0.852;
  mot.elec_spec.slip = 0;

  mot.feedback.type = mot_c::feedback_s::RES;
  mot.feedback.count = 1;
  mot.feedback.res_offset = 0.0;
  mot.noise.sin_scale = 1.0;
  mot.noise.cos_scale = 1.0;
  mot.noise.sin_offset = 0.0;
  mot.noise.cos_offset = 0.0;
  mot.noise.var = 0.001;

  mot.load.friction = 0.0;
  mot.load.load = 0.0;
  mot.load.damping = 0.0;
  mot.load.inertia = 0.0;


  cmd_c cmd;
  cmd.reset();
  cmd.periode = 2.0;
  cmd.amplitude = 1;
  cmd.wave = cmd_c::SQUARE;
  cmd.type = cmd_c::POS;
  cmd.pos_res = 0.01;
  cmd.vel_res = 0.01;
  cmd.acc_res = 0.01;

  drive_c drive;
  drive.reset();
  drive.dc = 50;
  drive.pwm_scale = 0.9;
  drive.pwm_res = 8400;
  drive.mot = &mot;
  drive.in = &cmd;
  drive.input = input;
  drive.pid = pid;
  drive.output = output;


  //cout << flush << "time cmd pos est_pos sin_avg sin_scale cos_avg cos_scale" << endl;
  cout << flush << "time cmd pos est_pos p v a ctr e" << endl;

  for(sim_time = 0.0; sim_time < sim_end_time; sim_time += sim_step){
    drive.in->step(sim_step);
    drive.step(sim_step);
    drive.mot->step(sim_step);
    //cout << sim_time << ", " << drive.in->get_pos() << ", " << drive.mot->state.pos << ", " << drive.est.pos << ", " << drive.est.sin_avg << ", " << drive.est.sin_scale << ", " << drive.est.cos_avg << ", " << drive.est.cos_scale << endl;
    cout << sim_time << ", " << drive.in->get_pos() << ", " << drive.mot->state.pos << ", " << drive.est.pos << ", " << drive.est.p << ", " << drive.est.v << ", " << drive.est.a << ", " << drive.state.ctr << ", " << minus_(drive.in->get_pos(), drive.est.pos) << endl;

  }

  system("gnuplot --persist gp");

  return(0);
}
