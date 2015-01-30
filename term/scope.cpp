#include "scope.hpp"

scope::scope(){
    sample_freq = 100.0;

    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        data_min[i] = INFINITY;
        data_max[i] = -INFINITY;
        data_avg[i] = 0.0;
        data_lp[i] = 0.0;
        
        gains[i] = 1.0;
        gain_freqs[i] = 100.0;
        gain_lp_const[i] = freq_to_lp_const(gain_freqs[i]);
        gain_modes[i] = FIXED;
        
        coupling_modes[i] = DC;
        coupling_freqs[i] = 100.0;
        coupling_lp_const[i] = freq_to_lp_const(coupling_freqs[i]);
        coupling_offsets[i] = 0.0;
        
        lowpass_freqs[i] = 0.0;
        lowpass_lp_const[i] = freq_to_lp_const(lowpass_freqs[i]);
    }
    
    buf_size = 1024;
    plot_size = 1024;
    plot_stepping = 1;
    sample_pos = 0;

    trigger_channel = 0;
    trigger_polarity = RISING;
    trigger_level = 1.0;
    trigger_coupling_mode = DC;
    trigger_coupling_freq = 100.0;
    trigger_coupling_lp_const = freq_to_lp_const(trigger_coupling_freq);
    trigger_lowpass_freq = 100.0;
    trigger_lowpass_lp_const = freq_to_lp_const(trigger_lowpass_freq);
    trigger_mode = CONTINOUS;
    trigger_deadtime = buf_size / sample_freq;
    trigger_delay = 0.1 * trigger_deadtime;
    trigger_state = WAITING_FOR_START;
    trigger_start_pos = 0;
    trigger_sleep_time = 0.0;
    triggered = false;
    
    running = false;
}

scope::~scope(){
}

float scope::freq_to_lp_const(float freq){
    return(1.0 / (sample_freq / (freq * M_PI_2 + 1.0)));
}

void scope::add(float values[NUM_OF_CHANNELS]){
    if(running){
        for(int i = 0; i < NUM_OF_CHANNELS; i++){
            data[sample_pos].channel[i] = values[i];
        }
        do_scope();
    
        sample_pos++;
        sample_pos %= buf_size;
    }
}

void scope::do_scope(){
    switch(trigger_mode){
        case OFF:
            break;
            
        case OFF_FLOATING:
            break;
            
        case CONTINOUS:
            switch(trigger_state) {
                case WAITING_FOR_TRIGGER:
                    do_trigger();
                    break;
                    
                case WAITING_FOR_DEADTIME:
                    std::cout << "do_scope: trigger in wrong state " << std::endl;
                    break;
                    
                case WAITING_FOR_BUFFER_END:
                    std::cout << "do_scope: trigger in wrong state " << std::endl;
                    break;
                    
                case WAITING_FOR_START:
                    break;
                    
                default:
                    break;
            }
            break;
            
        case CONTINOUS_DEADTIME:
            switch(trigger_state) {
                case WAITING_FOR_TRIGGER:
                    do_trigger();
                    break;
                    
                case WAITING_FOR_DEADTIME:
                    trigger_sleep_time -= 1.0 / sample_freq;
                    if(trigger_sleep_time <= 0.0){
                        trigger_sleep_time = 0.0;
                        trigger_state = WAITING_FOR_TRIGGER;
                        do_trigger();
                    }
                    break;
                
                case WAITING_FOR_BUFFER_END:
                    std::cout << "do_scope: trigger in wrong state " << std::endl;
                    break;

                
                case WAITING_FOR_START:
                    break;
                    
                default:
                    break;
            }
            break;
        
        case CONTINOUS_FULL_BUFFER:
            switch(trigger_state) {
                case WAITING_FOR_TRIGGER:
                    do_trigger();
                    break;
                    
                case WAITING_FOR_DEADTIME:
                    std::cout << "do_scope: trigger in wrong state " << std::endl;
                    break;
                    
                case WAITING_FOR_BUFFER_END:
                    //TODO
                    break;
                    
                case WAITING_FOR_START:
                    break;
                    
                default:
                    break;
            }
            break;
        
        case SINGLE_SHOT:
            switch(trigger_state) {
                case WAITING_FOR_TRIGGER:
                    do_trigger();
                    break;
                    
                case WAITING_FOR_DEADTIME:
                    std::cout << "do_scope: trigger in wrong state " << std::endl;
                    break;
                    
                case WAITING_FOR_BUFFER_END:
                    std::cout << "do_scope: trigger in wrong state " << std::endl;
                    break;
                    
                case WAITING_FOR_START:
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
    do_calc();
}

void scope::do_calc(){
    switch(trigger_mode) {
        case OFF:
        case OFF_FLOATING:
            // pdata[calc_pos - sample_pos] = lp(data[calc_pos - sample_pos]);
            calc_pos = sample_pos;
            if(trigger_mode == OFF){
                calc_start_pos = 0;
            }
            else{
                calc_start_pos++;
                calc_start_pos %= buf_size;
            }
            break;
        
        case CONTINOUS:
        case CONTINOUS_DEADTIME:
        case CONTINOUS_FULL_BUFFER:
        case SINGLE_SHOT:
            if(triggered){
                triggered = false;
                calc_start_pos = (trigger_start_pos + buf_size - (unsigned int)(trigger_delay * sample_freq)) % buf_size;
                calc_pos = trigger_start_pos;
                
                /*
                if calc_start_pos < sample_pos < trigger_start_pos{
                    pdata[calc_start_pos - sample_pos] = 0.0;
                    pdata[sample_pos - trigger_start_pos] = lp(data[sample_pos - trigger_start_pos]);
                }
                else{
                    pdata[calc_start_pos - trigger_start_pos] = lp(data[calc_start_pos - trigger_start_pos]);
                }
                pdata[trigger_start_pos - calc_start_pos] = 0.0;
                */
            }
            else{
                if(calc_pos != calc_start_pos){
                    //pdata[calc_pos] = data[calc_pos];
                    calc_pos++;
                    calc_pos %= buf_size;
                }
            }
            
        default:
            break;
    }
}

void scope::do_trigger(){
    static float value = 0.0;
    float last = value;
    static float avg;
    float level = trigger_level;
    
    value = trigger_lowpass_lp_const * data[sample_pos].channel[trigger_channel] + (1.0 - trigger_lowpass_lp_const) * value;
    avg = trigger_coupling_lp_const * data[sample_pos].channel[trigger_channel] + (1.0 - trigger_coupling_lp_const) * avg;
    
    switch(trigger_coupling_mode) {
        case AC:
            level -= avg;
            break;
            
        default:
            break;
    }
    
    bool fe = false;
    bool re = false;
    
    if(last < level && value >= level){
        re = true;
    }
    
    if(last >= level && value < level){
        fe = true;
    }
    
    switch(trigger_state) {
        case WAITING_FOR_TRIGGER:
            switch(trigger_polarity){
                case RISING:
                    if(re){
                        triggered = true;
                    }
                    break;
                    
                case FALLING:
                    if(fe){
                        triggered = true;
                    }
                    break;
                    
                case BOTH:
                    if(re || fe){
                        triggered = true;
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
    if(triggered){
        switch(trigger_mode){
            case OFF:
                std::cout << "do_trigger: wrong state" << std::endl;
                break;
                
            case OFF_FLOATING:
                std::cout << "do_trigger: wrong state" << std::endl;
                break;
                
            case CONTINOUS:
                trigger_state = WAITING_FOR_TRIGGER;
                break;
                
            case CONTINOUS_DEADTIME:
                trigger_state = WAITING_FOR_DEADTIME;
                break;
                
            case CONTINOUS_FULL_BUFFER:
                trigger_state = WAITING_FOR_BUFFER_END;
                break;
                
            default:
                break;
        }
    }
}

void scope::snapshot(){
    //gain, offset, coupling
    //plotdata[0 - plot_size] = pdata[calc_start_pos - calc_start_pos + plot_size * plot_stepping];
}