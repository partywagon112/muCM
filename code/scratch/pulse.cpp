void pulse_channels(){
  typedef enum {
    CHANNEL_A,
    CHANNEL_B,
    CHANNEL_C,
    CHANNEL_D,
    CHANNEL_LENGTH
  } channels_t;

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
