#include "conf_comp.h"
#include <stdio.h>
#include "main.h"  // for Wait
#include "commands.h"
#include "hal.h"

HAL_COMP(conf);

HAL_PIN(r); // phase resistance [ohm]
HAL_PIN(l); // phase inductance [H]
HAL_PIN(j); // motor inertia [kg*m^2]
HAL_PIN(d); // motor damping [Nm/rad/s]
HAL_PIN(f); // motor friction [Nm]
HAL_PIN(o); // motor offset [Nm]
HAL_PIN(j_sys); // system inertia [kg*m^2]
HAL_PIN(j_lpf); // motor to load bandwidth [Hz]
HAL_PIN(psi); // electrical torque constant [V*s/rad]
HAL_PIN(polecount); // #motor pole pairs
HAL_PIN(out_rev); // motor direction
HAL_PIN(high_mot_temp); // lower motor overtemperature limit [°C]
HAL_PIN(max_mot_temp); // upper motor overtemperature limit [°C]
HAL_PIN(phase_time); // autophasing time [s]
HAL_PIN(phase_cur); // autophasing current [A]

HAL_PIN(max_vel); // max. velocity [rad/s]
HAL_PIN(max_acc); // max. acceleration [rad/s^2]
HAL_PIN(max_force); // max. toruqe [Nm]
HAL_PIN(max_dc_cur); // max. dc current [A]
HAL_PIN(max_ac_cur); // max. phase current [A]

HAL_PIN(mot_fb_polecount); // #motor feedback pole pairs
HAL_PIN(mot_fb_offset); // motor feedback commutation offset [rad]
HAL_PIN(mot_fb_rev); // motor feedback direction
HAL_PIN(mot_fb_res); // motor feedback resolution [1/rev]

HAL_PIN(joint_fb_polecount); // #joint feedback pole pairs
HAL_PIN(joint_fb_offset); // joint feedback commutation offset [rad]
HAL_PIN(joint_fb_rev); // joint feedback direction
HAL_PIN(joint_fb_res); // joint feedback resolution [1/rev]

HAL_PIN(com_fb_polecount); // #commutation feedback pole pairs
HAL_PIN(com_fb_offset); // commutation feedback commutation offset [rad]
HAL_PIN(com_fb_rev); // commutation feedback direction
HAL_PIN(com_fb_res); // commutation feedback resolution [1/rev]

HAL_PIN(cmd_rev); // command direction
HAL_PIN(cmd_res); // command resolution [1/rev]

HAL_PIN(max_dc_volt); // upper dc link overvoltage limit [V]
HAL_PIN(max_hv_temp); // upper driver overtemperature limit [°C]
HAL_PIN(max_pos_error); // max position error [rad], 0 = disabled
HAL_PIN(high_dc_volt); // lower dc link overvoltage limit [V]
HAL_PIN(low_dc_volt); // dc link undervoltage limit [V]
HAL_PIN(high_hv_temp); // lower driver overtemperature limit [°C]
HAL_PIN(fan_hv_temp); // driver fan threshold [°C]
HAL_PIN(fan_mot_temp); // motor fan threshold [°C]

HAL_PIN(g); // motor model limit scaling
HAL_PIN(pos_bw); // position loop bandwidth [rad/s/rad]
HAL_PIN(vel_bw); // velocity loop bandwidth [rad/s^2/(rad/s)]
HAL_PIN(vel_d); // velocity loop damping
HAL_PIN(vel_g); // velocity loop proportional limit scaling
HAL_PIN(torque_g); // torque loop proportional limit scaling
HAL_PIN(cur_bw); // current loop bandwidth [V/A]
HAL_PIN(cur_ff); // current loop resistance feedforward gain
HAL_PIN(cur_ind); // current loop BEMF feedforward gain
HAL_PIN(max_sat); // max velocity, acceleration and torque saturation time [s]

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
