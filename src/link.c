#include "link.h"
#include "hal.h"
#include "misc.h"

void link_pid(){
   hal_set_pin("fault0.rt_prio", 3.0);
   hal_set_pin("rev0.rt_prio", 4.0);
   hal_set_pin("rev1.rt_prio", 4.0);
   hal_set_pin("vel0.rt_prio", 5.0);
   hal_set_pin("vel1.rt_prio", 5.0);
   hal_set_pin("stp0.rt_prio", 6.0);
   hal_set_pin("cauto0.rt_prio", 7.0);
   hal_set_pin("pid0.rt_prio", 8.0);
   hal_set_pin("t2c0.rt_prio", 9.0);
   hal_set_pin("curpid0.rt_prio", 10.0);
   hal_set_pin("pmsm0.rt_prio", 11.0);
   hal_set_pin("pmsm_limits0.rt_prio", 12.0);
   hal_set_pin("idq0.rt_prio", 13.0);
   hal_set_pin("hv0.rt_prio", 14.0);
   hal_set_pin("dq0.rt_prio", 14.1);
   hal_set_pin("sim0.rt_prio", 15.0);
   hal_set_pin("term0.rt_prio", 15.0);

   // cmd
   hal_link_pins("conf0.cmd_rev", "rev0.rev");
   hal_link_pins("vel0.pos_out", "net0.cmd");
   hal_link_pins("conf0.max_vel", "stp0.max_vel");
   hal_link_pins("conf0.max_acc", "stp0.max_acc");
   hal_link_pins("rev0.out", "vel0.pos_in");
   hal_link_pins("fault0.enable_pid", "vel0.en");
   hal_link_pins("vel0.vel", "net0.cmd_d");
   hal_set_pin("vel0.w", 500.0);
   hal_link_pins("net0.cmd","pid0.pos_ext_cmd");
   hal_link_pins("net0.cmd_d", "pid0.vel_ext_cmd");

   // fb
   hal_link_pins("conf0.fb_rev", "rev1.rev");

   hal_link_pins("rev1.out", "vel1.pos_in");
   hal_link_pins("fault0.enable_pid", "vel1.en");
   hal_link_pins("vel1.pos_out", "cauto0.fb_in");

   hal_link_pins("conf0.j", "vel1.j");
   hal_link_pins("pmsm0.torque", "vel1.torque");
   hal_link_pins("vel1.vel", "net0.fb_d");
   hal_link_pins("cauto0.fb_out", "net0.fb");

   hal_link_pins("net0.fb","pid0.pos_fb");
   hal_link_pins("net0.fb_d", "pid0.vel_fb");

   // fault
   hal_link_pins("conf0.max_dc_cur", "fault0.max_dc_cur");
   hal_link_pins("conf0.max_ac_cur", "fault0.max_ac_cur");
   hal_link_pins("conf0.max_dc_volt", "fault0.max_dc_volt");
   hal_link_pins("conf0.max_hv_temp", "fault0.max_hv_temp");
   hal_link_pins("conf0.max_core_temp", "fault0.max_core_temp");
   hal_link_pins("conf0.max_motor_temp", "fault0.max_motor_temp");
   hal_link_pins("conf0.max_pos_error", "fault0.max_pos_error");
   hal_link_pins("conf0.high_dc_volt", "fault0.high_dc_volt");
   hal_link_pins("conf0.high_hv_temp", "fault0.high_hv_temp");
   hal_link_pins("conf0.high_motor_temp", "fault0.high_motor_temp");
   hal_link_pins("conf0.low_dc_volt", "fault0.low_dc_volt");
   hal_link_pins("conf0.fan_hv_temp", "fault0.fan_hv_temp");
   hal_link_pins("conf0.fan_core_temp", "fault0.fan_core_temp");
   hal_link_pins("conf0.fan_motor_temp", "fault0.fan_motor_temp");
   hal_link_pins("conf0.autophase", "fault0.phase_on_start");
   hal_link_pins("conf0.max_sat", "fault0.max_sat");

   hal_set_pin("fault0.reset", 0.0);

   hal_link_pins("fault0.phase_start", "cauto0.start");
   hal_link_pins("cauto0.ready", "fault0.phase_ready");

   hal_link_pins("pid0.pos_error", "fault0.pos_error");
   hal_link_pins("pid0.saturated", "fault0.sat");
   hal_link_pins("hv0.dc_volt", "fault0.dc_volt");
   hal_link_pins("hv0.hv_temp", "fault0.hv_temp");
   hal_link_pins("net0.core_temp0", "fault0.core_temp0");
   hal_link_pins("net0.core_temp1", "fault0.core_temp1");
   hal_link_pins("net0.motor_temp", "fault0.motor_temp");
   hal_link_pins("hv0.dc_cur", "fault0.dc_cur");
   hal_link_pins("net0.fb_error", "fault0.fb_error");
   hal_link_pins("net0.cmd", "fault0.cmd");
   hal_link_pins("rev1.out", "fault0.fb");
   hal_link_pins("fault0.start_offset", "cauto0.start_offset");

   hal_link_pins("fault0.cur", "curpid0.max_cur");

   hal_link_pins("fault0.brake", "io0.brake");
   hal_link_pins("fault0.hv_fan", "io0.fan");

   hal_link_pins("fault0.enable_out", "hv0.enable");
   hal_link_pins("fault0.enable_pid", "pid0.enable");

   hal_link_pins("net0.enable", "fault0.enable");

   hal_link_pins("fault0.led_green", "io0.green");
   hal_link_pins("fault0.led_red", "io0.red");

   hal_link_pins("fault0.state", "term0.fault"); //sent fault to terminal
   // cauto

   hal_link_pins("conf0.fb_offset", "cauto0.mag_offset");
   hal_link_pins("conf0.phase_cur", "cauto0.cur");
   hal_link_pins("conf0.phase_time", "cauto0.time");

   // pid
   hal_link_pins("conf0.j", "pid0.j");
   hal_link_pins("conf0.p", "pid0.p");
   hal_link_pins("conf0.pos_p", "pid0.pos_p");
   hal_link_pins("conf0.vel_p", "pid0.vel_p");
   hal_link_pins("conf0.acc_p", "pid0.acc_p");
   hal_link_pins("conf0.acc_pi", "pid0.acc_pi");
   hal_link_pins("conf0.max_vel", "pid0.max_usr_vel");
   hal_link_pins("conf0.max_acc", "pid0.max_usr_acc");
   hal_link_pins("conf0.max_force", "pid0.max_usr_torque");
   hal_set_pin("pid0.pos_lp", 4000.0);
   hal_set_pin("pid0.vel_lp", 4000.0);
   hal_set_pin("pid0.acc_lp", 4000.0);
   hal_set_pin("pid0.torque_ff", 1.0);
   hal_set_pin("pid0.vel_ff", 1.0);
   hal_set_pin("pid0.acc_ff", 1.0);
   hal_set_pin("net0.enable", 1.0);
   hal_link_pins("pid0.torque_cor_cmd", "t2c0.torque");

   // t2c
   hal_link_pins("conf0.polecount", "t2c0.polecount");
   hal_link_pins("conf0.psi", "t2c0.psi");

   // curpid
   hal_link_pins("cauto0.i_d", "curpid0.id_cmd");
   hal_link_pins("t2c0.cur", "curpid0.iq_cmd");
   hal_link_pins("hv0.dc_volt", "curpid0.dc_volt");
   hal_link_pins("hv0.ac_volt", "curpid0.ac_volt");
   hal_link_pins("conf0.r", "curpid0.rd");
   hal_link_pins("conf0.r", "curpid0.rq");
   hal_link_pins("conf0.l", "curpid0.ld");
   hal_link_pins("conf0.l", "curpid0.lq");
   hal_link_pins("conf0.cur_ff", "curpid0.ff");
   hal_link_pins("conf0.cur_p", "curpid0.kp");
   hal_link_pins("conf0.cur_i", "curpid0.ki");
   hal_link_pins("conf0.cur_ind", "curpid0.kind");

   // pmsm
   hal_link_pins("curpid0.ud", "pmsm0.ud");
   hal_link_pins("curpid0.uq", "pmsm0.uq");
   hal_link_pins("net0.fb_d", "pmsm0.vel");
   hal_link_pins("conf0.r", "pmsm0.r");
   hal_link_pins("conf0.l", "pmsm0.ld");
   hal_link_pins("conf0.l", "pmsm0.lq");
   hal_link_pins("conf0.psi", "pmsm0.psi");
   hal_link_pins("pmsm0.id", "curpid0.id_fb");
   hal_link_pins("pmsm0.iq", "curpid0.iq_fb");
   hal_link_pins("pmsm0.indd", "curpid0.indd_fb");
   hal_link_pins("pmsm0.indq", "curpid0.indq_fb");
   hal_link_pins("conf0.polecount", "pmsm0.polecount");
   hal_link_pins("pmsm0.iq", "hv0.iq");

   // pmsm_limits
   hal_link_pins("conf0.r", "pmsm_limits0.r");
   hal_link_pins("conf0.l", "pmsm_limits0.ld");
   hal_link_pins("conf0.l", "pmsm_limits0.lq");
   hal_link_pins("conf0.psi", "pmsm_limits0.psi");
   hal_link_pins("conf0.polecount", "pmsm_limits0.polecount");
   hal_link_pins("hv0.ac_volt", "pmsm_limits0.ac_volt");
   hal_link_pins("pmsm0.indq", "pmsm_limits0.indq");
   hal_link_pins("pmsm0.iq", "pmsm_limits0.iq");
   hal_link_pins("pmsm_limits0.next_max_torque", "pid0.max_torque");
   hal_link_pins("pmsm_limits0.next_min_torque", "pid0.min_torque");
   hal_link_pins("pmsm_limits0.abs_max_vel", "pid0.max_vel");

   //troller only
   hal_link_pins("conf0.polecount", "dq0.polecount");
   hal_link_pins("cauto0.pos", "dq0.pos");
   hal_link_pins("hv0.iu", "dq0.u");
   hal_link_pins("hv0.iv", "dq0.v");
   hal_link_pins("hv0.iw", "dq0.w");

   hal_link_pins("curpid0.ud", "idq0.d");
   hal_link_pins("curpid0.uq", "idq0.q");
   hal_link_pins("conf0.polecount", "idq0.polecount");
   hal_link_pins("idq0.a", "hv0.a");
   hal_link_pins("idq0.b", "hv0.b");

   // misc
   hal_link_pins("conf0.out_rev", "hv0.rev");
   hal_link_pins("cauto0.pos", "idq0.pos");
   hal_link_pins("conf0.cmd_res", "sim0.res");

   // term
   hal_link_pins("net0.cmd", "term0.wave0");
   hal_link_pins("net0.fb", "term0.wave1");
   hal_link_pins("net0.cmd_d", "term0.wave2");
   hal_link_pins("net0.fb_d", "term0.wave3");
   hal_set_pin("term0.gain0", 20.0);
   hal_set_pin("term0.gain1", 20.0);
   hal_set_pin("term0.gain2", 1.0);
   hal_set_pin("term0.gain3", 1.0);

   hal_link_pins("conf0.sin_gain", "adc0.sin_gain");
   hal_link_pins("conf0.cos_gain", "adc0.cos_gain");
   hal_link_pins("conf0.sin_offset", "adc0.sin_offset");
   hal_link_pins("conf0.cos_offset", "adc0.cos_offset");
}

int update_fb(){
   hal_set_pin("adc0.rt_prio", -1.0);
   hal_set_pin("enc_fb0.rt_prio", -1.0);
   hal_set_pin("res0.rt_prio", -1.0);
   hal_set_pin("encm0.rt_prio", -1.0);
   hal_set_pin("encs0.rt_prio", -1.0);
   hal_set_pin("yaskawa0.rt_prio", -1.0);
   switch((protocol_t)hal_get_pin("conf0.fb_type")){
      case ENC:
         hal_link_pins("enc_fb0.pos", "rev1.in");
         hal_link_pins("conf0.fb_res", "enc_fb0.res");
         hal_set_pin("enc_fb0.rt_prio", 2.0);
         break;
      case RES:
         hal_link_pins("res0.pos", "rev1.in");
         hal_link_pins("conf0.fb_polecount", "res0.poles");
         hal_link_pins("adc0.sin", "res0.sin");
         hal_link_pins("adc0.cos", "res0.cos");
         hal_link_pins("adc0.quad", "res0.quad");
         hal_link_pins("res0.error", "net0.fb_error");
         hal_set_pin("adc0.res_en", 1.0);
         hal_set_pin("adc0.rt_prio", 1.0);
         hal_set_pin("res0.rt_prio", 2.0);
         break;
      case SINCOS:
         hal_link_pins("adc0.sin3", "enc_fb0.sin");
         hal_link_pins("adc0.cos3", "enc_fb0.cos");
         hal_link_pins("adc0.quad", "enc_fb0.quad");
         hal_link_pins("enc_fb0.ipos", "rev1.in");
         hal_link_pins("conf0.fb_res", "enc_fb0.res");
         hal_link_pins("enc_fb0.error", "net0.fb_error");
         hal_set_pin("enc_fb0.ires", 1024.0);
         hal_set_pin("adc0.rt_prio", 1.0);
         hal_set_pin("enc_fb0.rt_prio", 2.0);
         break;
      case MITSU:
         hal_link_pins("encm0.pos", "rev1.in");
         hal_link_pins("encm0.error", "net0.fb_error");
         hal_set_pin("encm0.rt_prio", 2.0);
         break;
      case YASKAWA:
         hal_link_pins("yaskawa0.pos", "rev1.in");
         hal_set_pin("yaskawa0.rt_prio", 2.0);
         break;
      case SANYO:
         hal_link_pins("encs0.pos", "rev1.in");
         hal_set_pin("encs0.rt_prio", 2.0);
         break;
      default:
         return -1;
   }
   return 0;
}

int update_cmd(){
   hal_set_pin("enc_cmd0.rt_prio", -1.0);
   hal_set_pin("sserial0.rt_prio", -1.0);
   hal_set_pin("sserial0.frt_prio", -1.0);
   hal_set_pin("en0.rt_prio", -1.0);
   switch((protocol_t)hal_get_pin("conf0.cmd_type")){
      case ENC:
         hal_link_pins("enc_cmd0.pos", "rev0.in");
         hal_link_pins("conf0.cmd_res", "enc_cmd0.res");
         hal_set_pin("enc_cmd0.rt_prio", 2.0);
         if(hal_get_pin("conf0.error_out") == 1.0){//error out using rs485
            hal_set_pin("en0.rt_prio", 15.0);
            hal_set_pin("en0.en", 1.0);//TODO: link to error pin
            hal_set_pin("en0.txen", 1.0);
         }
         break;
      case SSERIAL:
         hal_link_pins("sserial0.pos_cmd", "rev0.in");
         //this breaks cmd rev...
         //net0.cmd_d <= sserial0.pos_cmd_d
         hal_set_pin("sserial0.rt_prio", 2.0);
         hal_set_pin("sserial0.frt_prio", 2.0);
         break;
      default:
         return -1;
   }
   return 0;
}
