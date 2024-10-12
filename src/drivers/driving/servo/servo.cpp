#include "drivers/driving/servo/servo.h"
#include "pico/stdlib.h"

#define SRVSIG 13
static uint slice_num;
static uint32_t pwm_wrap;

void init_SG90_servo(){
    gpio_set_function(SRVSIG,GPIO_FUNC_PWM);

    slice_num = pwm_gpio_to_slice_num(SRVSIG);
    pwm_wrap = (125000000 / PWM_FREQUENCY) - 1;
    pwm_set_wrap(slice_num, pwm_wrap);
}

void set_SG90_servo_pos(uint16_t pulse_width){
    uint32_t pwm_compare = (pulse_width * pwm_wrap) / (1000000 / PWM_FREQUENCY);

    pwm_set_chan_level(slice_num, PWM_CHAN_A, pwm_compare);

}