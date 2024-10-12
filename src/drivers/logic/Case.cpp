#include <stdio.h>
#include "drivers/logic/Case.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

static volatile int switchpos = 0;
void switch_isr(uint gpio, uint32_t events){
        switchpos = (switchpos + 1) % 3;
}

void switch_init(){
    gpio_init(EMGNCY_SW);
    gpio_set_dir(EMGNCY_SW, GPIO_IN);
    gpio_set_irq_enabled_with_callback(EMGNCY_SW,GPIO_IRQ_EDGE_FALL, true, &switch_isr);
}

int get_switch_state(void){
    return switchpos;
}