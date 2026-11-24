#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include <string.h>
#include <led_clock_driver.h>
#include <colors.h>

led_color_t deactive_color = {.r = 0x00, .g = 0x00, .b = 0x00};

char current_str[5] = {'0'};

void clock_set_symbol(led_clock_t *clock, char symbol, uint8_t position)
{
    uint8_t newBytes = SYMBOLS[symbol - '0'];
    size_t offset = clock->leds_in_segment * 7 * position;
    led_color_t active_color = clock->color;

    for (int i = 0; i < clock->leds_in_segment * 7; i++)
    {
        if (newBytes & (1 << i / clock->leds_in_segment))
        {
            led_set_pixel(clock->strip, active_color, offset + i);
        }
        else
        {
            led_set_pixel(clock->strip, deactive_color, offset + i);
        }
    }
};

esp_err_t led_clock_init(led_clock_t *clock, led_strip_t *strip, uint8_t leds_in_segment)
{
    if (!clock)
        return ESP_ERR_INVALID_ARG;

    clock->strip = strip;
    clock->leds_in_segment = leds_in_segment;
    clock->color = WHITE;
    clock->brightness = 0x05;

    led_set_brightness(strip, clock->brightness);
    led_clear(clock->strip);
    return ESP_OK;
}

void clock_set_color(led_clock_t *clock, uint32_t color)
{
    led_color_t new_color = {.r = color & 0xFF0000, .g = color & 0x00FF00, .b = color & 0x0000FF};
    led_set_color(clock->strip, new_color);
};

void clock_set_brightness(led_clock_t *clock, uint8_t brightness)
{
    led_set_brightness(clock->strip, brightness);
};

void clock_set_time(led_clock_t *clock, time_t time)
{
    struct tm *now = localtime(&time);

    strftime(current_str, sizeof(current_str), "%M%S", now);
    ESP_LOGI("LED_CLOCK", "new time %s", current_str);

    clock_set_symbol(clock, current_str[0], 0);
    clock_set_symbol(clock, current_str[1], 1);
    clock_set_symbol(clock, current_str[2], 2);
    clock_set_symbol(clock, current_str[3], 3);
    led_show(clock->strip);
};