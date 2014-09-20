typedef struct input{
  char name[32];

  enum{
    ENC,
    STEP_DIR,
    RES,
    GMS,
    ETH,
    RS485,
    MISC,
    CONST_POS,
    CONST_VEL,
    SIN_POS,
    SIN_VEL
    SIN_ACC
  } class;

  int port;

  int (*setup)(input* i);

  enum{
    INC,
    ABS
  } type;

  float periode;

  enum{
    POS,
    POS_VEL,
    POS_VEL_ACC
    VEL,
    VEL_ACC,
    ACC
  } flag;

  int (*init)(input* i);
  int (*deinit)(input* i);

  void (*reset)(input *i);

  int (*start)(input* i);
  int (*stop)(input* i);

  int (*valid)(input* i);

  void (*work)(input* i);
  void (*filter)(input* i);
  void (*read)(input* i);

  float (*get_pos)(input* i);
  float (*get_vel)(input* i);
  float (*get_acc)(input* i);

  float pos[32];
  float vel[32];
  float acc[32];
  int pointer;

  enum{
    PREINIT,
    DISABLED,
    STARTING,
    RUNNING,
    ERROR
  } state;

  char info[32];
};

int init(input* i){
  i->state = DISABLED;
  i->reset(i);
  return(1);
}

int deinit(input* i){
  i->state = PREINIT;
  return(1);
}

void reset(input* i){
  for(int j = 0; j < 32; j++){
    i->pos[j] = 0.0;
    i->pos[j] = 0.0;
    i->pos[j] = 0.0;
  }
}

int start(input* i){
  if(i->state == DISABLED || i->state == RUNNING){
    i->state = RUNNING;
    return(1);
  }
  return(0);
}

int stop(input* i){
  if(i->state == DISABLED || i->state == RUNNING){
    i->state = DISABLED;
    return(1);
  }
  return(0);
}

int valid(input* i){
  if(i->state == RUNNING){
    return(1);
  }
  return(0);
}

void work(input* i){
  i->pointer = ++(i->pointer) % 32;
  i->read(i);
  i->filter(i);
}

void filter1(input* i){
  i->vel[i->pointer] = (i->pos[i->pointer] - i->pos[(i->pointer + 31) % 32]) / i->periode[i->pointer];
  i->acc[i->pointer] = (i->vel[i->pointer] - i->vel[(i->pointer + 31) % 32]) / i->periode[i->pointer];
}

void filter2(input* i){
  i->vel[i->pointer] = (i->pos[i->pointer] - i->pos[(i->pointer + 1) % 32]) / i->periode[i->pointer];
  i->acc[i->pointer] = (i->vel[i->pointer] - i->vel[(i->pointer + 1) % 32]) / i->periode[i->pointer];
}

void read(input* i){
  i->pos[i->pointer] = 0.0;
}

float get_pos(input* i){
  return(i->pos[i->pointer]);
}

float get_vel(input* i){
  return(i->vel[i->pointer]);
}

float get_acc(input* i){
  return(i->acc[i->pointer]);
}

int setup(input *i){

}


void read_const_vel(input* i){
  i->pos[i->pointer] = 1.0 * i->periode[i->pointer];
}

void read_enc_tim3(input* i){
  i->pos[i->pointer] = TIM_GetCounter(TIM3) * 2 * pi / 2048;
}

void read_gms(input* i){
  static int min[2] = {65535, 65535}, max[2] = {-65535, -65535};
  int value[2], mid[2];
  if(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){
    i->info = "ADC1 not ready\0";
    i->state = ERROR;
    return;
  }
  if(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)){
    i->info = "ADC2 not ready\0";
    i->state = ERROR;
    return;
  }

  value[0] = ADC_GetConversionValue(ADC1);
  value[1] = ADC_GetConversionValue(ADC2);

  min[0] = MIN(value[0], min[0]);
  min[1] = MIN(value[1], min[1]);
  max[0] = MAX(value[0], max[0]);
  max[1] = MAX(value[1], max[1]);


  mid[0] = (min[0] + max[0]) / 2;
  mid[1] = (min[1] + max[1]) / 2;

  i->pos[i->pointer] = atan2f(value[0] - mid[0], value[1] - mid[1]);

  ADC_SoftwareStartConv(ADC1);
  ADC_SoftwareStartConv(ADC2);
}
