#ifndef MAGNETICSWITCH_H
#define MAGNETICSWITCH_H

#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <cstdint>

void init_Mag_Switch(uint32_t Mag_GPIO_Pin);
bool Mag_Switch_Active_True(uint32_t Mag_GPIO_Pin);

#endif