#define ADC2V(a) ((a) / (ARES) * (AREF))


/*
SIG
 o
 |
.-.
| | pu
'-'
 |----o ADC
.-.
| | pd
'-'
 |
===
GND
*/
#define V0(a, pd, pu) ((a) / (pd) * ((pu) + (pd)))


/*
        VCC
         +
         |
        .-.
        | | pu
        '-'
         |----o ADC
        .-.
        | | pm
        '-'
SIG o----|
        .-.
        | | shunt
        '-'
         |
        ===
        GND
*/
#define V1(a, vcc, pu, pm) ((a) + (pm) * ((vcc) - (a)) / (pu))
#define AMP(a, vcc, pu, pm, shunt, gain) (V2((gain) * (a), vcc, pu, pm) / (shunt))
//#define AMP(a, gain, vcc, shunt, pu, pm) (((a) / (gain) - (vcc) / ((pu) + (pm)) * (pm)) / ((shunt) * (pu)) * ((pu) + (pm)))


/*
            VCC
             +
             |
            .-.
            | | pu
            '-'
       ___   |
SIG o-|___|--+----o ADC
       pm    |
            .-.
            | | pd
            '-'
             |
            ===
            GND
*/
#define V2(a, vcc, pu, pm, pd) (((a) - (vcc) / ((pu) + (pd)) * (pd)) / (1.0 / (1.0 / (pu) + 1.0 / (pd))) * ((pm) + (1.0 / (1.0 / (pu) + 1.0 / (pd)))) + (vcc) / ((pu) + (pd)) * (pd))


/*
        VCC
         +
         |
        .-.
        | | pu
        '-'
SIG o----|
        .-.
        | | pm
        '-'
         |----o ADC
        .-.
        | | pd
        '-'
         |
        ===
        GND
*/
#define V3(a, pm, pd) ((a) / (pd) * ((pm) + (pd)))
#define R3(a, vcc, pu, pm, pd) (V3(a, pm, pd) / ((vcc)-V3(a, pm, pd)) / (pu)-V3(a, pm, pd) / ((pm) + (pd)))