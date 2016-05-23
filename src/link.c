#include "link.h"
#include "hal.h"
#include "misc.h"

void link_pid(){
   set_hal_pin("fault0.rt_prio", 3.0);
   set_hal_pin("rev0.rt_prio", 4.0);
   set_hal_pin("rev1.rt_prio", 4.0);
   set_hal_pin("vel0.rt_prio", 5.0);
   set_hal_pin("vel1.rt_prio", 5.0);
   set_hal_pin("stp0.rt_prio", 6.0);
   set_hal_pin("cauto0.rt_prio", 7.0);
   set_hal_pin("pid0.rt_prio", 8.0);
   set_hal_pin("t2c0.rt_prio", 9.0);
   set_hal_pin("curpid0.rt_prio", 10.0);
   set_hal_pin("pmsm0.rt_prio", 11.0);
   set_hal_pin("pmsm_limits0.rt_prio", 12.0);
   set_hal_pin("idq0.rt_prio", 13.0);
   set_hal_pin("hv0.rt_prio", 14.0);
   set_hal_pin("dq0.rt_prio", 14.1);
   set_hal_pin("sim0.rt_prio", 15.0);
   set_hal_pin("term0.rt_prio", 15.0);

   // cmd
   link_hal_pins("conf0.cmd_rev", "rev0.rev");
   link_hal_pins("vel0.pos_out", "net0.cmd");
   link_hal_pins("conf0.max_vel", "stp0.max_vel");
   link_hal_pins("conf0.max_acc", "stp0.max_acc");
   link_hal_pins("rev0.out", "vel0.pos_in");
   link_hal_pins("fault0.enable_pid", "vel0.en");
   link_hal_pins("vel0.vel", "net0.cmd_d");
   set_hal_pin("vel0.w", 500.0);
   link_hal_pins("net0.cmd","pid0.pos_ext_cmd");
   link_hal_pins("net0.cmd_d", "pid0.vel_ext_cmd");

   // fb
   link_hal_pins("conf0.fb_rev", "rev1.rev");

   link_hal_pins("rev1.out", "vel1.pos_in");
   link_hal_pins("fault0.enable_pid", "vel1.en");
   link_hal_pins("vel1.pos_out", "cauto0.fb_in");

   link_hal_pins("conf0.j", "vel1.j");
   link_hal_pins("pmsm0.torque", "vel1.torque");
   link_hal_pins("vel1.vel", "net0.fb_d");
   link_hal_pins("cauto0.fb_out", "net0.fb");

   link_hal_pins("net0.fb","pid0.pos_fb");
   link_hal_pins("net0.fb_d", "pid0.vel_fb");

   // fault
   link_hal_pins("conf0.max_dc_cur", "fault0.max_dc_cur");
   link_hal_pins("conf0.max_ac_cur", "fault0.max_ac_cur");
   link_hal_pins("conf0.max_dc_volt", "fault0.max_dc_volt");
   link_hal_pins("conf0.max_hv_temp", "fault0.max_hv_temp");
   link_hal_pins("conf0.max_core_temp", "fault0.max_core_temp");
   link_hal_pins("conf0.max_motor_temp", "fault0.max_motor_temp");
   link_hal_pins("conf0.max_pos_error", "fault0.max_pos_error");
   link_hal_pins("conf0.high_dc_volt", "fault0.high_dc_volt");
   link_hal_pins("conf0.high_hv_temp", "fault0.high_hv_temp");
   link_hal_pins("conf0.high_motor_temp", "fault0.high_motor_temp");
   link_hal_pins("conf0.low_dc_volt", "fault0.low_dc_volt");
   link_hal_pins("conf0.fan_hv_temp", "fault0.fan_hv_temp");
   link_hal_pins("conf0.fan_core_temp", "fault0.fan_core_temp");
   link_hal_pins("conf0.fan_motor_temp", "fault0.fan_motor_temp");
   link_hal_pins("conf0.autophase", "fault0.phase_on_start");
   link_hal_pins("conf0.max_sat", "fault0.max_sat");

   set_hal_pin("fault0.reset", 0.0);

   link_hal_pins("fault0.phase_start", "cauto0.start");
   link_hal_pins("cauto0.ready", "fault0.phase_ready");

   link_hal_pins("pid0.pos_error", "fault0.pos_error");
   link_hal_pins("pid0.saturated", "fault0.sat");
   link_hal_pins("hv0.dc_volt", "fault0.dc_volt");
   link_hal_pins("hv0.hv_temp", "fault0.hv_temp");
   link_hal_pins("net0.core_temp0", "fault0.core_temp0");
   link_hal_pins("net0.core_temp1", "fault0.core_temp1");
   link_hal_pins("net0.motor_temp", "fault0.motor_temp");
   link_hal_pins("hv0.dc_cur", "fault0.dc_cur");
   link_hal_pins("net0.fb_error", "fault0.fb_error");
   link_hal_pins("net0.cmd", "fault0.cmd");
   link_hal_pins("rev1.out", "fault0.fb");
   link_hal_pins("fault0.start_offset", "cauto0.start_offset");

   link_hal_pins("fault0.cur", "curpid0.max_cur");

   link_hal_pins("fault0.brake", "io0.brake");
   link_hal_pins("fault0.hv_fan", "io0.fan");

   link_hal_pins("fault0.enable_out", "hv0.enable");
   link_hal_pins("fault0.enable_pid", "pid0.enable");

   link_hal_pins("net0.enable", "fault0.enable");

   link_hal_pins("fault0.led_green", "io0.green");
   link_hal_pins("fault0.led_red", "io0.red");

   link_hal_pins("fault0.state", "term0.fault"); //sent fault to terminal
   // cauto

   link_hal_pins("conf0.fb_offset", "cauto0.mag_offset");
   link_hal_pins("conf0.phase_cur", "cauto0.cur");
   link_hal_pins("conf0.phase_time", "cauto0.time");

   // pid
   link_hal_pins("conf0.j", "pid0.j");
   link_hal_pins("conf0.p", "pid0.p");
   link_hal_pins("conf0.pos_p", "pid0.pos_p");
   link_hal_pins("conf0.vel_p", "pid0.vel_p");
   link_hal_pins("conf0.acc_p", "pid0.acc_p");
   link_hal_pins("conf0.acc_pi", "pid0.acc_pi");
   link_hal_pins("conf0.max_vel", "pid0.max_usr_vel");
   link_hal_pins("conf0.max_acc", "pid0.max_usr_acc");
   link_hal_pins("conf0.max_force", "pid0.max_usr_torque");
   set_hal_pin("pid0.pos_lp", 4000.0);
   set_hal_pin("pid0.vel_lp", 4000.0);
   set_hal_pin("pid0.acc_lp", 4000.0);
   set_hal_pin("pid0.torque_ff", 1.0);
   set_hal_pin("pid0.vel_ff", 1.0);
   set_hal_pin("pid0.acc_ff", 1.0);
   set_hal_pin("net0.enable", 1.0);
   link_hal_pins("pid0.torque_cor_cmd", "t2c0.torque");

   // t2c
   link_hal_pins("conf0.polecount", "t2c0.polecount");
   link_hal_pins("conf0.psi", "t2c0.psi");

   // curpid
   link_hal_pins("cauto0.i_d", "curpid0.id_cmd");
   link_hal_pins("t2c0.cur", "curpid0.iq_cmd");
   link_hal_pins("hv0.dc_volt", "curpid0.dc_volt");
   link_hal_pins("hv0.ac_volt", "curpid0.ac_volt");
   link_hal_pins("conf0.r", "curpid0.rd");
   link_hal_pins("conf0.r", "curpid0.rq");
   link_hal_pins("conf0.l", "curpid0.ld");
   link_hal_pins("conf0.l", "curpid0.lq");
   link_hal_pins("conf0.cur_ff", "curpid0.ff");
   link_hal_pins("conf0.cur_p", "curpid0.kp");
   link_hal_pins("conf0.cur_i", "curpid0.ki");
   link_hal_pins("conf0.cur_ind", "curpid0.kind");

   // pmsm
   link_hal_pins("curpid0.ud", "pmsm0.ud");
   link_hal_pins("curpid0.uq", "pmsm0.uq");
   link_hal_pins("net0.fb_d", "pmsm0.vel");
   link_hal_pins("conf0.r", "pmsm0.r");
   link_hal_pins("conf0.l", "pmsm0.ld");
   link_hal_pins("conf0.l", "pmsm0.lq");
   link_hal_pins("conf0.psi", "pmsm0.psi");
   link_hal_pins("pmsm0.id", "curpid0.id_fb");
   link_hal_pins("pmsm0.iq", "curpid0.iq_fb");
   link_hal_pins("pmsm0.indd", "curpid0.indd_fb");
   link_hal_pins("pmsm0.indq", "curpid0.indq_fb");
   link_hal_pins("conf0.polecount", "pmsm0.polecount");
   link_hal_pins("pmsm0.iq", "hv0.iq");

   // pmsm_limits
   link_hal_pins("conf0.r", "pmsm_limits0.r");
   link_hal_pins("conf0.l", "pmsm_limits0.ld");
   link_hal_pins("conf0.l", "pmsm_limits0.lq");
   link_hal_pins("conf0.psi", "pmsm_limits0.psi");
   link_hal_pins("conf0.polecount", "pmsm_limits0.polecount");
   link_hal_pins("hv0.ac_volt", "pmsm_limits0.ac_volt");
   link_hal_pins("pmsm0.indq", "pmsm_limits0.indq");
   link_hal_pins("pmsm0.iq", "pmsm_limits0.iq");
   link_hal_pins("pmsm_limits0.next_max_torque", "pid0.max_torque");
   link_hal_pins("pmsm_limits0.next_min_torque", "pid0.min_torque");
   link_hal_pins("pmsm_limits0.abs_max_vel", "pid0.max_vel");

   //troller only
   link_hal_pins("conf0.polecount", "dq0.polecount");
   link_hal_pins("cauto0.pos", "dq0.pos");
   link_hal_pins("hv0.iu", "dq0.u");
   link_hal_pins("hv0.iv", "dq0.v");
   link_hal_pins("hv0.iw", "dq0.w");

   link_hal_pins("curpid0.ud", "idq0.d");
   link_hal_pins("curpid0.uq", "idq0.q");
   link_hal_pins("conf0.polecount", "idq0.polecount");
   link_hal_pins("idq0.a", "hv0.a");
   link_hal_pins("idq0.b", "hv0.b");

   // misc
   link_hal_pins("conf0.out_rev", "hv0.rev");
   link_hal_pins("cauto0.pos", "idq0.pos");
   link_hal_pins("conf0.cmd_res", "sim0.res");

   // term
   link_hal_pins("net0.cmd", "term0.wave0");
   link_hal_pins("net0.fb", "term0.wave1");
   link_hal_pins("net0.cmd_d", "term0.wave2");
   link_hal_pins("net0.fb_d", "term0.wave3");
   set_hal_pin("term0.gain0", 20.0);
   set_hal_pin("term0.gain1", 20.0);
   set_hal_pin("term0.gain2", 1.0);
   set_hal_pin("term0.gain3", 1.0);

   link_hal_pins("conf0.sin_gain", "adc0.sin_gain");
   link_hal_pins("conf0.cos_gain", "adc0.cos_gain");
   link_hal_pins("conf0.sin_offset", "adc0.sin_offset");
   link_hal_pins("conf0.cos_offset", "adc0.cos_offset");
}

int update_fb(){
   set_hal_pin("adc0.rt_prio", -1.0);
   set_hal_pin("enc_fb0.rt_prio", -1.0);
   set_hal_pin("res0.rt_prio", -1.0);
   set_hal_pin("encm0.rt_prio", -1.0);
   set_hal_pin("encs0.rt_prio", -1.0);
   set_hal_pin("yaskawa0.rt_prio", -1.0);
   switch((protocol_t)get_hal_pin("conf0.fb_type")){
      case ENC:
         link_hal_pins("enc_fb0.pos", "rev1.in");
         link_hal_pins("conf0.fb_res", "enc_fb0.res");
         set_hal_pin("enc_fb0.rt_prio", 2.0);
         break;
      case RES:
         link_hal_pins("res0.pos", "rev1.in");
         link_hal_pins("conf0.fb_polecount", "res0.poles");
         link_hal_pins("adc0.sin", "res0.sin");
         link_hal_pins("adc0.cos", "res0.cos");
         link_hal_pins("adc0.quad", "res0.quad");
         link_hal_pins("res0.error", "net0.fb_error");
         set_hal_pin("adc0.res_en", 1.0);
         set_hal_pin("adc0.rt_prio", 1.0);
         set_hal_pin("res0.rt_prio", 2.0);
         break;
      case SINCOS:
         link_hal_pins("adc0.sin3", "enc_fb0.sin");
         link_hal_pins("adc0.cos3", "enc_fb0.cos");
         link_hal_pins("adc0.quad", "enc_fb0.quad");
         link_hal_pins("enc_fb0.ipos", "rev1.in");
         link_hal_pins("conf0.fb_res", "enc_fb0.res");
         link_hal_pins("enc_fb0.error", "net0.fb_error");
         set_hal_pin("enc_fb0.ires", 1024.0);
         set_hal_pin("adc0.rt_prio", 1.0);
         set_hal_pin("enc_fb0.rt_prio", 2.0);
         break;
      case MITSU:
         link_hal_pins("encm0.pos", "rev1.in");
         link_hal_pins("encm0.error", "net0.fb_error");
         set_hal_pin("encm0.rt_prio", 2.0);
         break;
      case YASKAWA:
         link_hal_pins("yaskawa0.pos", "rev1.in");
         set_hal_pin("yaskawa0.rt_prio", 2.0);
         break;
      case SANYO:
         link_hal_pins("encs0.pos", "rev1.in");
         set_hal_pin("encs0.rt_prio", 2.0);
         break;
      default:
         return -1;
   }
   return 0;
}

int update_cmd(){
   set_hal_pin("enc_cmd0.rt_prio", -1.0);
   set_hal_pin("sserial0.rt_prio", -1.0);
   set_hal_pin("sserial0.frt_prio", -1.0);
   set_hal_pin("en0.rt_prio", -1.0);
   switch((protocol_t)get_hal_pin("conf0.cmd_type")){
      case ENC:
         link_hal_pins("enc_cmd0.pos", "rev0.in");
         link_hal_pins("conf0.cmd_res", "enc_cmd0.res");
         set_hal_pin("enc_cmd0.rt_prio", 2.0);
         if(get_hal_pin("conf0.error_out") == 1.0){//error out using rs485
            set_hal_pin("en0.rt_prio", 15.0);
            set_hal_pin("en0.en", 1.0);//TODO: link to error pin
            set_hal_pin("en0.txen", 1.0);
         }
         break;
      case SSERIAL:
         link_hal_pins("sserial0.pos_cmd", "vel_int0.pos_in");
         link_hal_pins("sserial0.pos_cmd_d", "vel_int0.vel_in");
         
         //this breaks cmd rev...
         link_hal_pins("vel_int0.pos_out", "net0.cmd");
         link_hal_pins("vel_int0.vel_out", "net0.cmd_d");
         set_hal_pin("vel_int0.wd", 0.002);//TODO: this depends on linuxcnc servo thread period
         //TODO: handle error of vel_int
         set_hal_pin("sserial0.rt_prio", 2.0);
         set_hal_pin("sserial0.frt_prio", 2.0);
         set_hal_pin("vel_int0.rt_prio", 2.1);
         
         break;
      default:
         return -1;
   }
   return 0;
}
