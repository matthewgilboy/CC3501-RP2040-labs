#include "magneticswitch.h"

// Initialise Magnetic Switch (Sensor)
void init_Mag_Switch(uint32_t Mag_GPIO_Pin){
    gpio_init(Mag_GPIO_Pin);
    gpio_set_dir(Mag_GPIO_Pin, GPIO_IN);
    gpio_pull_up(Mag_GPIO_Pin);
}

bool Mag_Switch_Active_True(uint32_t Mag_GPIO_Pin){
    return !gpio_get(Mag_GPIO_Pin);
}