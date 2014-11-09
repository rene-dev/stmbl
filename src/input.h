#pragma once

struct input_{
  //name_ name;

  enum{
    QUAD,
    RES,
    GMS,
    //STEP_DIR,
    //ETHERNET,
    //RS232,
    SIM,
    MISC
  } type;

  enum{
    POS,
    POS_VEL,
    POS_VEL_ACC,
    VEL,
    VEL_ACC,
    ACC,
    FORCE
  } type2;

  enum{
    PRE_INIT,
    DISABLED,
    STARTING,
    ENABLED,
    FERROR
  } state;

  int num;

  float scale;

  float pos;
  float vel;
  float acc;
  float force;

  char info[32];

  int (*init)(struct input_* i);
  int (*enable)(struct input_* i);
  int (*disable)(struct input_* i);
  int (*reset)(struct input_* i);
  float (*get_pos)(struct input_* i);
  float (*get_vel)(struct input_* i);
  float (*get_acc)(struct input_* i);
  float (*get_force)(struct input_* i);
  void (*do_rt_stuff)(struct input_* i, float period);
};

typedef struct input_ input;
