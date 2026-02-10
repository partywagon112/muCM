#include <Arduino.h>
#include <Matter.h>
#include <MatterLightbulb.h>
#include <Wire.h>
// #include <PD_UFP.h>
#include "drv8955.h"

#define PD_ENABLE

#define SERIAL_SPEED 115200
#define FUSB302_ADDRESS = 0x23  // This is a constant, but in hope
                                // I fix this, it can live here.

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

// PD_UFP_c PD_UFP;

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
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  PD_UFP.init_PPS(FUSB302_nINT_PIN, PPS_V(9), PPS_A(1));
  pinMode(LED_PIN, OUTPUT);
  pinMode(IN0_PIN, ANALOG);

  // Leave it off for now.
  Driver.sleep();

  Serial.begin(SERIAL_SPEED);
}

void loop() {
  // PD_UFP.run();

  // if (PD_UFP.is_PPS_ready()){
  //   Serial.printf("is pps ready %d\r\n");
  //   digitalWrite(LED_PIN, HIGH);
  // }
  
  Serial.printf("nFault = %d\r\n", digitalRead(nFAULT_PIN));
  Serial.printf("IO0 = %d \r\n", analogRead(IN0_PIN));

  delay(1000);
}