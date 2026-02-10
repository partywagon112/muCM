void setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  PD_UFP.init_PPS(FUSB302_nINT_PIN, PPS_V(9), PPS_A(1));
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  pinMode(nSLEEP_PIN, OUTPUT);
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);
  pinMode(MODE_PIN, INPUT);
  pinMode(nFAULT_PIN, INPUT_PULLUP);

  digitalWrite(nSLEEP_PIN, HIGH);
  digitalWrite(PWM1_PIN, HIGH);
  digitalWrite(EN1_PIN, HIGH);
  // Driver.set_ch_a_on();
  // Driver.set_ch_b_on();
  // Driver.set_ch_c_off();
  // Driver.set_ch_d_off();
  delay(1000);
  digitalWrite(nSLEEP_PIN, LOW);
  delay(100);
  digitalWrite(nSLEEP_PIN, HIGH);
}