// Driver class for DRV8955. 
// 
//
// Example usage:
// 
// Driver_4CH Driver(
//   EN1_PIN,
//   PWM1_PIN,  
//   EN2_PIN,
//   PWM2_PIN,  
//   EN4_PIN,
//   PWM4_PIN,  
//   EN3_PIN,
//   PWM3_PIN,  
//   nSLEEP_PIN,
//   nFAULT_PIN
// );
// 
// ...
// 
// void loop()

#include <Arduino.h>


class Driver_4CH {
  public:
    typedef enum{
      CH_A,
      CH_B,
      CH_C,
      CH_D,
      CH_COUNT
    } driver_channels_t;

    typedef enum{
      ENABLE=HIGH,
      DISABLE=LOW
    } driver_state_t;

    Driver_4CH(
      int ch_a_en_pin,
      int ch_a_pwm_pin,  
      int ch_b_en_pin,
      int ch_b_pwm_pin,  
      int ch_c_en_pin,
      int ch_c_pwm_pin, 
      int ch_d_en_pin,    
      int ch_d_pwm_pin, 
      int nsleep_pin,
      int nfault_pin
    ) : 
      ch_a_en(ch_a_en_pin),
      ch_a_pwm(ch_a_pwm_pin),
      ch_b_en(ch_b_en_pin),
      ch_b_pwm(ch_b_pwm_pin),
      ch_c_en(ch_c_en_pin),
      ch_c_pwm(ch_c_pwm_pin),
      ch_d_en(ch_d_en_pin),
      ch_d_pwm(ch_d_pwm_pin),
      nsleep(nsleep_pin),
      nfault(nfault_pin)
    {
      ch_a_fault = 0;
      ch_b_fault = 0;
      ch_c_fault = 0;
      ch_d_fault = 0;

      pinMode(ch_a_en, OUTPUT);
      pinMode(ch_a_pwm, OUTPUT);
      pinMode(ch_b_en, OUTPUT);
      pinMode(ch_b_pwm, OUTPUT);
      pinMode(ch_c_en, OUTPUT);
      pinMode(ch_c_pwm, OUTPUT);
      pinMode(ch_d_en, OUTPUT);
      pinMode(ch_d_pwm, OUTPUT);
      pinMode(nsleep, OUTPUT);
      pinMode(nfault, INPUT);
      // Mode not set, want high z operation.

      sleep(0);

      clear_past_faults();
    };

    void sleep(){digitalWrite(nsleep, LOW);};               // Overload to make syntax shorter.
    void sleep(bool state){digitalWrite(nsleep, !state);};
    bool get_fault(){return digitalRead(!nfault);};

    bool set_ch_a_on(){return set_ch(CH_A, ENABLE, 255);};
    bool set_ch_b_on(){return set_ch(CH_B, ENABLE, 255);};
    bool set_ch_c_on(){return set_ch(CH_C, ENABLE, 255);};
    bool set_ch_d_on(){return set_ch(CH_D, ENABLE, 255);};

    bool set_ch_a_pwm(int pwm){return set_ch(CH_A, ENABLE, pwm);};
    bool set_ch_b_pwm(int pwm){return set_ch(CH_B, ENABLE, pwm);};
    bool set_ch_c_pwm(int pwm){return set_ch(CH_C, ENABLE, pwm);};
    bool set_ch_d_pwm(int pwm){return set_ch(CH_D, ENABLE, pwm);};

    bool set_ch_a_off(){return set_ch(CH_A, ENABLE, 0);};
    bool set_ch_b_off(){return set_ch(CH_B, ENABLE, 0);};
    bool set_ch_c_off(){return set_ch(CH_C, ENABLE, 0);};
    bool set_ch_d_off(){return set_ch(CH_D, ENABLE, 0);};
    
    bool set_ch_a_HiZ(){return set_ch(CH_A, DISABLE, 0);};
    bool set_ch_b_HiZ(){return set_ch(CH_B, DISABLE, 0);};
    bool set_ch_c_HiZ(){return set_ch(CH_C, DISABLE, 0);};
    bool set_ch_d_HiZ(){return set_ch(CH_D, DISABLE, 0);};

    // protos
    bool past_fault_present();
    ulong get_fault_ts(driver_channels_t channel);
    void clear_past_faults();
    void run();

  private:
    int ch_a_en;
    int ch_a_pwm; 
    int ch_b_en;
    int ch_b_pwm; 
    int ch_c_en;
    int ch_c_pwm;
    int ch_d_en;
    int ch_d_pwm;
    int nsleep;
    int nfault;

    ulong ch_a_fault;
    ulong ch_b_fault;
    ulong ch_c_fault;
    ulong ch_d_fault;

    ulong stamp_fault(driver_channels_t channel);
    bool check_fault();
    bool set_ch(driver_channels_t channel, driver_state_t state, int pwm);
};


