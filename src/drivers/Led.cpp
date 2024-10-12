#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "LED.h"
#include "WS2812.pio.h"

#define LED_PIN 14
#define NUM_LEDS 12
#define MAX_HUE 360  // Maximum hue value for rainbow
#define TILT_THRESHOLD 5  // Minimum tilt angle in degrees to change LED position
#define MAX_TILT_ANGLE 80
//LAB7

void initialise_led(uint32_t *led_data, int i, uint8_t sm, uint8_t red, uint8_t green, uint8_t blue){
    if (i < NUM_LEDS) {
        led_data[i] = (red << 24) | (green << 16) | (blue << 8);
    }
}

void update_leds(uint32_t *led_data, int sm) {
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio0, sm, led_data[i]);
    }
}

void turn_off_all(uint32_t *led_data) {
    for (int i = 0; i < NUM_LEDS; i++){
        led_data[i] = 0;
    }
}

// New function to turn all LEDs red
void turnAllWhite(uint32_t *led_data, int sm) {
    for (int i = 0; i < NUM_LEDS; i++) {
        initialise_led(led_data, i, sm, 255, 255, 255);  // Red color
    }
    update_leds(led_data, sm);
}

void hsv_to_rgb(uint16_t hue, uint8_t *r, uint8_t *g, uint8_t *b) {
    uint8_t region = hue / 60;
    uint8_t remainder = (hue - (region * 60)) * 255 / 60;

    uint8_t p = 0;
    uint8_t q = (255 * (60 - remainder)) / 60;
    uint8_t t = (255 * remainder) / 60;

    switch (region) {
        case 0:
            *r = 255; *g = t; *b = p;
            break;
        case 1:
            *r = q; *g = 255; *b = p;
            break;
        case 2:
            *r = p; *g = 255; *b = t;
            break;
        case 3:
            *r = p; *g = q; *b = 255;
            break;
        case 4:
            *r = t; *g = p; *b = 255;
            break;
        case 5:
        default:
            *r = 255; *g = p; *b = q;
            break;
    }
}

void rainbow(uint32_t *led_data, int sm, uint16_t *hue_offset) {
    uint8_t r, g, b;
    uint16_t hue;
    for (int i = 0; i < NUM_LEDS; i++) {
        hue = (*hue_offset + (i * MAX_HUE / NUM_LEDS)) % MAX_HUE;
        hsv_to_rgb(hue, &r, &g, &b);
        initialise_led(led_data, i, sm, r, g, b);
    }
    update_leds(led_data, sm);

    // Increment hue offset for next cycle
    (*hue_offset) += 5;  // Adjust speed by changing the increment value
    if (*hue_offset >= MAX_HUE) {
        *hue_offset = 0;
    }
}

// LAB8
void init_pio_led(){ 
uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
uint32_t led_data[NUM_LEDS] = {0}; }// Array to hold data for 12 LEDs

// Function to determine which LED to light based on tilt angle
int mapTiltToLED(float tilt) {
    // Normalize tilt to range 0 to 4 for four LEDs (0: extreme negative tilt, 4: extreme positive tilt)
    int ledIndex = (int)((tilt + MAX_TILT_ANGLE) / (2 * MAX_TILT_ANGLE / 4));
    if (ledIndex < 0) return 0;
    if (ledIndex > 3) return 3;
    return ledIndex;
}

// Function to update LEDs for tilt emulation with paired left and right LEDs
void updateTiltLEDs(uint32_t *led_data, int sm, float xTilt, float yTilt) {
    // Determine which LEDs to light on each side
    int sideLED = mapTiltToLED(xTilt);  // Left and right sides (paired)
    int frontLED = mapTiltToLED(yTilt); // Front side LEDs (independent)

    // Clear all LEDs
    turn_off_all(led_data);

    // Set LEDs for paired left (blue) and right (green) sides based on xTilt
    for (int i = 0; i < 4; i++) {
        // Flip the logic: now, tilting left lights up green LEDs on the right
        if (i == sideLED) {  // Right side (flipped to green for left tilt)
            initialise_led(led_data, i, sm, 0, 255, 0);  // Green for right tilt (flipped)
        }
        if (i == (3 - sideLED)) {  // Left side (flipped to blue for right tilt)
            initialise_led(led_data, i + 8, sm, 0, 255, 0);  // Blue for left tilt (flipped)
        }
    }

    // Set LEDs for back side (blue) independently based on yTilt (flipped forward/backward)
    for (int i = 4; i < 8; i++) {
        // Flip the logic: tilting forward lights up LEDs that would normally be for backward tilt
        if (i - 4 == frontLED) {  // Backward side (flipped to blue for forward tilt)
            initialise_led(led_data, i, sm, 0, 0, 255);  // Blue for backward tilt (flipped)
        }
    }

    // Update LED display
    update_leds(led_data, sm);
}