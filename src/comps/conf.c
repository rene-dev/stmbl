#include <stdio.h>
#include "main.h"  // for Wait
#include "commands.h"
#include "hal.h"

HAL_COMP(conf);

HAL_PIN(r);
HAL_PIN(l);
HAL_PIN(j);
HAL_PIN(psi);
HAL_PIN(polecount);
HAL_PIN(mot_type);  //ac sync,async/dc,2phase
HAL_PIN(out_rev);
HAL_PIN(high_mot_temp);
HAL_PIN(max_mot_temp);
HAL_PIN(phase_time);
HAL_PIN(phase_cur);

HAL_PIN(max_vel);
HAL_PIN(max_acc);
HAL_PIN(max_force);
HAL_PIN(max_dc_cur);
HAL_PIN(max_ac_cur);

HAL_PIN(mot_fb_polecount);
HAL_PIN(mot_fb_offset);
HAL_PIN(mot_fb_rev);
HAL_PIN(mot_fb_res);

HAL_PIN(joint_fb_polecount);
HAL_PIN(joint_fb_offset);
HAL_PIN(joint_fb_rev);
HAL_PIN(joint_fb_res);

HAL_PIN(com_fb_polecount);
HAL_PIN(com_fb_offset);
HAL_PIN(com_fb_rev);
HAL_PIN(com_fb_res);

HAL_PIN(autophase);  //constant,cauto,hfi

HAL_PIN(cmd_rev);
HAL_PIN(cmd_res);
//HAL_PIN(pos_static);//track pos in disabled and error condition TODO: not implemented

HAL_PIN(max_dc_volt);
HAL_PIN(max_hv_temp);
HAL_PIN(max_core_temp);
HAL_PIN(max_pos_error);
HAL_PIN(high_dc_volt);
HAL_PIN(low_dc_volt);
HAL_PIN(high_hv_temp);
HAL_PIN(fan_hv_temp);
HAL_PIN(fan_core_temp);
HAL_PIN(fan_mot_temp);

HAL_PIN(g);
HAL_PIN(pos_p);
HAL_PIN(vel_p);
HAL_PIN(vel_i);
HAL_PIN(vel_g);
HAL_PIN(cur_p);
HAL_PIN(cur_i);
HAL_PIN(cur_ff);
HAL_PIN(cur_ind);
HAL_PIN(max_sat);

hal_comp_t conf_comp_struct = {
    .name      = "conf",
    .nrt       = 0,
    .rt        = 0,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct conf_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
