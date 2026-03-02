#include "drv8955.h"
#include <Arduino.h>

/*
Public
*/
bool Driver_4CH::past_fault_present(){
    for (driver_channels_t ch = CH_A; ch < CH_COUNT; ch=(driver_channels_t)(ch+1)){
    if (get_fault_ts(ch) > 0) return true;
    }
    return false;
}

ulong Driver_4CH::get_fault_ts(driver_channels_t channel){
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
    return 0;
}

void Driver_4CH::clear_past_faults(){
    ch_a_fault = 0;
    ch_b_fault = 0;
    ch_c_fault = 0;
    ch_d_fault = 0;
}

void Driver_4CH::run(){
    check_fault();
}


/*
Private:
*/

ulong Driver_4CH::stamp_fault(driver_channels_t channel){
    ulong currtime = millis();
    switch (channel){
        case CH_A:
            ch_a_fault = currtime;
            break;
        case CH_B:
            ch_b_fault = currtime;
            break;
        case CH_C:
            ch_c_fault = currtime;
            break;
        case CH_D:
            ch_d_fault = currtime;
            break;
    }
    return currtime;
}

bool Driver_4CH::check_fault(){
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

bool Driver_4CH::set_ch(driver_channels_t channel, driver_state_t state, int pwm){
    if (pwm < 0 || pwm > 255) return 1; // return PWM error
    switch(channel){
        case CH_A:
            digitalWrite(ch_a_en, state);
            analogWrite(ch_a_pwm, pwm);
            break;
        case CH_B:
            digitalWrite(ch_b_en, state);
            analogWrite(ch_b_pwm, pwm);
            break;
        case CH_C:
            digitalWrite(ch_c_en, state);
            analogWrite(ch_c_pwm, pwm);
            break;
        case CH_D:
            digitalWrite(ch_d_en, state);
            analogWrite(ch_d_pwm, pwm);
            break;
    }
    return check_fault();
}