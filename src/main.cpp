#include "pico/stdlib.h"
#include <stdio.h>
#include "GPIO_PINS.h"  // Assuming SRVSIG is defined here
#include "Includes.h"
#define HIGH_TIME_MS 2   // 2 milliseconds
#define PERIOD_MS 20     // Total period of 20 milliseconds

int main(){

InitialisePins();
setMicroStep(1);

for(;;){

	RunMotor(1);
        sleep_ms(10);
        RunMotor(0);
        sleep_ms(10);
}	
return 0;
}
