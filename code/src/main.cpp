#include <Arduino.h>
#include <Matter.h>
#include "drv8955.h"

#define SERIAL_SPEED 9600
#define FUSB302_ADDRESS = 0x23  // This is a constant, but in hope
                                // I fix this, it can live here.

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

Driver_4CH Driver(
  EN1_PIN,
  PWM1_PIN,  
  EN2_PIN,
  PWM2_PIN,  
  EN4_PIN,
  PWM4_PIN,  
  EN3_PIN,
  PWM3_PIN,  
  nSLEEP_PIN,
  nFAULT_PIN
);

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Leave it off for now.
  Driver.sleep(false);

  Serial.begin(SERIAL_SPEED);

  Serial.printf("Booted\r\n");

}

typedef enum {
  CHANNEL_A,
  CHANNEL_B,
  CHANNEL_C,
  CHANNEL_D,
  CHANNEL_LENGTH
} channels_t;

void loop() {
  while (true){
    Serial.printf("nFault = %d\r\n", digitalRead(nFAULT_PIN));

    Driver.set_ch_c_on();
    delay(1000);
    Driver.set_ch_c_off();
    delay(1000);

    for (channels_t i = CHANNEL_A; i < CHANNEL_LENGTH; i = channels_t((int)i + 1)){
      switch(i){
        case CHANNEL_A:
          printf("STATE A\r\n");
          Driver.set_ch_a_on();
          Driver.set_ch_b_off();
          Driver.set_ch_c_off();
          Driver.set_ch_d_off();
          break;
        case CHANNEL_B:
          printf("STATE B\r\n");
          Driver.set_ch_a_off();
          Driver.set_ch_b_on();
          Driver.set_ch_c_off();
          Driver.set_ch_d_off();
          break;
        case CHANNEL_C:
          printf("STATE C\r\n");
          Driver.set_ch_a_off();
          Driver.set_ch_b_off();
          Driver.set_ch_c_on();
          Driver.set_ch_d_off();
          break;
        case CHANNEL_D:
          printf("STATE D\r\n");
          Driver.set_ch_a_off();
          Driver.set_ch_b_off();
          Driver.set_ch_c_off();
          Driver.set_ch_d_on();
          break;
      }
      delay(1000);
    }

  }
}
