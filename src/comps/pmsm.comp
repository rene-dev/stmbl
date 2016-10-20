HAL_COMP(pmsm);

//in
HAL_PIN(psi) = 0.01;
HAL_PIN(r) = 1.0;
HAL_PIN(ld) = 0.001;
HAL_PIN(lq) = 0.001;
HAL_PIN(polecount) = 1.0;

HAL_PIN(vel) = 0.0;
HAL_PIN(ud) = 0.0;
HAL_PIN(uq) = 0.0;

HAL_PIN(indd) = 0.0;
HAL_PIN(indq) = 0.0;

//out
HAL_PIN(id) = 0.0;
HAL_PIN(iq) = 0.0;
HAL_PIN(psi_d) = 0.0;
HAL_PIN(psi_q) = 0.0;
HAL_PIN(torque) = 0.0;
HAL_PIN(drop_q) = 0.0;
HAL_PIN(drop_d) = 0.0;
HAL_PIN(drop_v) = 0.7;
HAL_PIN(drop_exp) = 0.04;




MEM(float id) = 0.0;
MEM(float iq) = 0.0;

// TODO: ifdef Troller, move to curpid
float drop(float i, float v){
   if(i < -v){
      return(-1.0);
   }
   if(i > v){
      return(1.0);
   }
   return(i / v);
}

RT(
   float p = (int)MAX(PIN(polecount), 1.0);
   float vel_e = PIN(vel) * p;
   float ld = MAX(PIN(ld), 0.0001);
   float lq = MAX(PIN(lq), 0.0001);
   float ud = PIN(ud);
   float uq = PIN(uq);
   float psi_m = MAX(PIN(psi), 0.01);
   float r = MAX(PIN(r), 0.01);

   float psi_d = ld * id + psi_m;
   float psi_q = lq * iq;

   float indd = vel_e * psi_q;
   float indq = vel_e * psi_d;
   
   float dropv = PIN(drop_v);
   float drope = PIN(drop_exp);
   float dropq = dropv * drop(iq, drope);
   float dropd = dropv * drop(id, drope);
   
   uq -= dropq;
   ud -= dropd;
   
   
   id += (ud - r * id + indd) / ld * period / 4.0;
   iq += (uq - r * iq - indq) / lq * period / 4.0;

   id += (ud - r * id + indd) / ld * period / 4.0;
   iq += (uq - r * iq - indq) / lq * period / 4.0;

   id += (ud - r * id + indd) / ld * period / 4.0;
   iq += (uq - r * iq - indq) / lq * period / 4.0;

   id += (ud - r * id + indd) / ld * period / 4.0;
   iq += (uq - r * iq - indq) / lq * period / 4.0;

   float t = 3.0 / 2.0 * p * (psi_m * iq + (ld - lq) * id * iq);

   PIN(id) = id;
   PIN(iq) = iq;
   PIN(indd) = indd;
   PIN(indq) = indq;
   PIN(psi_d) = psi_d;
   PIN(psi_q) = psi_q;
   PIN(torque) = t;
   
   PIN(drop_q) = dropq;
   PIN(drop_d) = dropd;
   
);

ENDCOMP;
