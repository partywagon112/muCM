#include <Arduino.h>
#include <Wire.h>
#include <PD_UFP.h>

// Check address is set to 0x23 not 0x22
#define FUSB302_nINT_PIN 11
#define LED_PIN 15
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

#define IN0_PIN 17
#define PWM1_PIN 12
#define PWM2_PIN 13
#define PWM3_PIN 21
#define PWM4_PIN 47
#define EN1_PIN 39
#define EN2_PIN 40
#define EN3_PIN 41
#define EN4_PIN 42
#define nSLEEP_PIN 10
#define nFAULT_PIN 2
#define MODE_PIN 48

PD_UFP_c PD_UFP;

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
      int ch_a_en,
      int ch_a_pwm,  
      int ch_b_en,
      int ch_b_pwm,  
      int ch_c_en,
      int ch_c_pwm, 
      int ch_d_en,    
      int ch_d_pwm, 
      int nsleep,
      int nfault
    ){
      ch_a_en = ch_a_en;
      ch_a_pwm =  ch_a_pwm;  
      ch_b_en = ch_b_en;
      ch_b_pwm =  ch_b_pwm; 
      ch_c_en = ch_c_en;
      ch_c_pwm =  ch_c_pwm;
      ch_d_en = ch_d_en;
      ch_d_pwm =  ch_d_pwm;
      nsleep = nsleep;
      nfault = nfault;

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

    int sleep() {digitalWrite(nsleep, 0);};
    int sleep(bool state) {digitalWrite(nsleep, !state);};
    bool get_fault(){return digitalRead(!nfault);};

    bool set_ch_a_on(){return set_ch(CH_A, ENABLE, 255);};
    bool set_ch_b_on(){return set_ch(CH_B, ENABLE, 255);};
    bool set_ch_c_on(){return set_ch(CH_C, ENABLE, 255);};
    bool set_ch_d_on(){return set_ch(CH_D, ENABLE, 255);};

    bool set_ch_a_off(){return set_ch(CH_A, ENABLE, 0);};
    bool set_ch_b_off(){return set_ch(CH_B, ENABLE, 0);};
    bool set_ch_d_off(){return set_ch(CH_C, ENABLE, 0);};
    bool set_ch_c_off(){return set_ch(CH_D, ENABLE, 0);};
    
    bool set_ch_a_HiZ(){return set_ch(CH_A, DISABLE, 0);};
    bool set_ch_b_HiZ(){return set_ch(CH_B, DISABLE, 0);};
    bool set_ch_c_HiZ(){return set_ch(CH_C, DISABLE, 0);};
    bool set_ch_d_HiZ(){return set_ch(CH_D, DISABLE, 0);};

    bool past_fault_present(){
      for (driver_channels_t ch = CH_A; ch < CH_COUNT; ch=(driver_channels_t)(ch+1)){
        if (get_fault_ts(ch) > 0) return true;
      }
      return false;
    }

    ulong get_fault_ts(driver_channels_t channel){
      switch(channel){
        case CH_A:
          return ch_a_fault;
        case CH_B:
          return ch_b_fault;
        case CH_C:
          return ch_c_fault;
        case CH_D:
          return ch_d_fault;
        }
    }

    void clear_past_faults(){
      ch_a_fault = 0;
      ch_b_fault = 0;
      ch_c_fault = 0;
      ch_d_fault = 0;
    }

    void run(){
      check_fault();
    }

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
    
    ulong stamp_fault(driver_channels_t channel){
      ulong currtime = millis();
      switch (channel){
        case CH_A:
          ch_a_fault = currtime;
        case CH_B:
          ch_b_fault = currtime;
        case CH_C:
          ch_c_fault = currtime;
        case CH_D:
          ch_d_fault = currtime;
      }
      return currtime;
    }

    bool check_fault(){
      if (nfault == 0){
        // turn off all drivers? try and hunt out the bad Driver sequentially??
        Serial.printf("Fault detected, starting shutdown process\r\n");
        for (driver_channels_t ch = CH_A; ch < CH_COUNT; ch=(driver_channels_t)(ch+1)){
          set_ch(ch, DISABLE, 0);
          Serial.printf("\t- disabled %d of %d\r\n", ch, CH_COUNT);
          if (check_fault()){
            // check again, might be gone, lets log that.
            Serial.printf("\t  fault not cleared\r\n");
            millis();
          } else {
            Serial.printf("\t  fault cleared!\r\n");
          }
        }
        Serial.printf("Finished fault shutdown procedure.");
        return true;
      }
      return false;
    }

    bool set_ch(driver_channels_t channel, driver_state_t state, int pwm){
      if (pwm < 0 || pwm > 255) return 1; // return PWM error
      switch(channel){
        case CH_A:
          digitalWrite(ch_a_en, state);
          analogWrite(ch_a_pwm, pwm);
        case CH_B:
          digitalWrite(ch_b_en, state);
          analogWrite(ch_b_pwm, pwm);
        case CH_C:
          digitalWrite(ch_b_en, state);
          analogWrite(ch_b_pwm, pwm);
        case CH_D:
          digitalWrite(ch_b_en, state);
          analogWrite(ch_b_pwm, pwm);
      }
      // return get_fault();
    }
};


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

void setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  PD_UFP.init_PPS(FUSB302_nINT_PIN, PPS_V(20), PPS_A(1));
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  pinMode(nSLEEP_PIN, OUTPUT);
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);
  pinMode(MODE_PIN, INPUT);

  digitalWrite(nSLEEP_PIN, HIGH);
  digitalWrite(PWM1_PIN, HIGH);
  digitalWrite(EN1_PIN, HIGH);
  // Driver.set_ch_a_on();
  // Driver.set_ch_b_on();
  // Driver.set_ch_c_off();
  // Driver.set_ch_d_off();
}

void loop() {
  PD_UFP.run();
  // Driver.run();
  if (PD_UFP.is_PPS_ready()){
    Serial.printf("is pps ready %d\r\n");
    digitalWrite(LED_PIN, HIGH);
  }
  
  // delay(100);
  // if (PD_UFP.is_PPS_ready())
  // {
  //   Serial.write("PPS trigger succcess\n");
  //   digitalWrite(LED_PIN, HIGH);
  // }
  // else if (PD_UFP.is_power_ready())
  // {
  //   Serial.write("Fail to trigger PPS\n");
  // }
  // else
  // {
  //   Serial.write("No PD supply available\n");
  // }
}