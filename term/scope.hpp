#pragma once

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/bitmap.h>
#include <wx/artprov.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/button.h>
#include <math.h>
#include <vector>
#include <libserialport.h>

#define NUM_OF_CHANNELS 4

class scope{
public:
    enum trg_pol{
        RISING,
        FALLING,
        BOTH
    };
    
    enum trg_mode{
        OFF,
        OFF_FLOATING,
        SINGLE_SHOT,
        CONTINOUS,
        CONTINOUS_FULL_BUFFER,
        CONTINOUS_DEADTIME
    };
    
    enum coupling_mode{
        ZERO,
        DC,
        AC,
        DC_OFFSET
    };
    
    enum gain_mode{
        FIXED,
        AUTO,
        MAX
    };
    
    struct values{
        float channel[NUM_OF_CHANNELS];
    };
    
    std::vector<values> plotdata;
    
    void add(float values[NUM_OF_CHANNELS]);
    
    void set_trigger_channel(unsigned int channel);
    void set_trigger_polarity(trg_pol polyrity);
    void set_trigger_level(float level);
    void set_trigger_coupling(coupling_mode mode);
    void set_trigger_coupling_freq(float freq);
    void set_trigger_lowpass_freq(float freq);
    void set_trigger_mode(trg_mode mode);
    void set_trigger_deadtime(float deadtime);
    void set_trigger_delay(float delay);
    void set_start_trigger();
    void set_stop_trigger();
    
    void set_coupling_mode(unsigned int channel, coupling_mode mode);
    void set_coupling_freq(unsigned int channel, float freq);
    void set_coupling_offset(unsigned int channel, float offset);
    
    void set_gain_mode(unsigned int channel, gain_mode mode);
    void set_gain_freq(unsigned int channel, float freq);
    void set_gain_gain(unsigned int channel, float gain);
    
    void set_lowpass_freq(unsigned int channel, float freq);
    
    void snapshot();
    void start_scope();
    void stop_scope();
    
    
    void set_sample_freq(float freq);
    void set_buffer_size(unsigned int size);
    void set_plot_size(unsigned int size);
    void set_plot_stepping(unsigned int stepping);
    
    scope();
    ~scope();
    
private:
    std::vector<values> data;
    std::vector<values> pdata;
    
    float data_min[NUM_OF_CHANNELS];
    float data_max[NUM_OF_CHANNELS];
    float data_avg[NUM_OF_CHANNELS];
    float data_lp[NUM_OF_CHANNELS];
    
    unsigned int buf_size;
    unsigned int plot_size;
    unsigned int plot_stepping;
    float sample_freq;
    
    float gains[NUM_OF_CHANNELS];
    float gain_freqs[NUM_OF_CHANNELS];
    float gain_lp_const[NUM_OF_CHANNELS];
    gain_mode gain_modes[NUM_OF_CHANNELS];
    
    coupling_mode coupling_modes[NUM_OF_CHANNELS];
    float coupling_freqs[NUM_OF_CHANNELS];
    float coupling_lp_const[NUM_OF_CHANNELS];
    float coupling_offsets[NUM_OF_CHANNELS];
    
    float lowpass_freqs[NUM_OF_CHANNELS];
    float lowpass_lp_const[NUM_OF_CHANNELS];

    unsigned int trigger_channel;
    trg_pol trigger_polarity;
    float trigger_level;
    trg_mode trigger_mode;
    float trigger_deadtime;
    float trigger_delay;
    coupling_mode trigger_coupling_mode;
    float trigger_coupling_freq;
    float trigger_coupling_lp_const;
    float trigger_lowpass_freq;
    float trigger_lowpass_lp_const;
    enum trg_state{
        WAITING_FOR_START,
        WAITING_FOR_DEADTIME,
        WAITING_FOR_BUFFER_END,
        WAITING_FOR_TRIGGER
    } trigger_state;
    float trigger_sleep_time;
    bool triggered;
    unsigned int trigger_start_pos;
    
    unsigned int calc_start_pos;
    unsigned int calc_pos;
    
    bool running;
    
    unsigned int sample_pos;
    unsigned int psample_pos;
    
    void do_scope();
    void do_trigger();
    void do_calc();
    
    float freq_to_lp_const(float freq);
};
