#pragma once

//fault state
typedef enum{
   DISABLED = 0,
   ENABLED,
   PHASING,
   SOFT_FAULT,
   HARD_FAULT
} state_t;

//fault errors
typedef enum{
   NO_ERROR = 0,
   CMD_ERROR,
   FB0_ERROR,
   FB1_ERROR,
   POS_ERROR,
   SAT_ERROR,
   HV_ERROR,
   HV_TEMP_ERROR,
   HV_VOLT_ERROR,
   MOT_TEMP_ERROR,
} fault_t;

void Wait(uint32_t ms);
