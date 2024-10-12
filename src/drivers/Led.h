#ifndef LED_H
#define LED_H
#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255
#include <stdint.h>  // For uint8_t, uint16_t types

// LED control functions
void initialise_pio();  // Initializes PIO for WS2812 LEDs
void initialise_led(uint32_t *led_data, int i, uint8_t sm, uint8_t red, uint8_t green, uint8_t blue);  // Initializes a specific LED with color
void update_leds(uint32_t *led_data, int sm);  // Updates all LEDs based on led_data array
void turn_off_all(uint32_t *led_data);  // Turns off all LEDs
void turnAllWhite(uint32_t *led_data, int sm);  // Turns all LEDs red
void hsv_to_rgb(uint16_t hue, uint8_t *r, uint8_t *g, uint8_t *b);  // Converts HSV to RGB color format
void rainbow(uint32_t *led_data, int sm, uint16_t *hue_offset);  // Creates a rainbow effect on LEDs
int mapTiltToLED(float tilt);  // Maps a tilt angle to a specific LED index
void updateTiltLEDs(uint32_t *led_data, int sm, float xTilt, float yTilt);  // Updates LEDs based on tilt data
void init_pio_led();  
#endif  // LED_H