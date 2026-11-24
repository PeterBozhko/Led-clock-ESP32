#ifndef LED_CLOCK_DRIVER_H
#define LED_CLOCK_DRIVER_H

#pragma once
#include <stdint.h>
#include <led_strip_driver.h>
#include <time.h>
// #include "driver/rmt_tx.h"

/*
0b07654321
   3      ---
 4   2   |   |
   1      ---
 5   7   |   |
   6      ---
*/
static const uint8_t SYMBOLS[10] = {
    0b01111110, // 0
    0b01000010, // 1
    0b00110111, // 2
    0b01100111, // 3
    0b01001011, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b01000110, // 7
    0b01111111, // 8
    0b01101111, // 9
};

typedef struct {
    led_strip_t *strip;          // Указатель на ленту
    uint8_t leds_in_segment;        // Количество светодиодов в 1 сегменте
    led_color_t color;         // Цвет
    uint8_t brightness;       // Общая яркость (0..255)
    
} led_clock_t;

esp_err_t led_clock_init(led_clock_t *clock, led_strip_t *strip, uint8_t leds_in_segment);
void clock_set_color(led_clock_t *clock, uint32_t color);
void clock_set_brightness(led_clock_t *clock, uint8_t brightness);
void clock_set_time(led_clock_t *clock, time_t time);
#endif