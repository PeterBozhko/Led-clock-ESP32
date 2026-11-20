#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
// #include "driver/rmt_tx.h"
#include <string.h>
#include <led_clock_driver.h>
#include <colors.h>

led_color_t deactive_color = {.r = 0x00, .g = 0x00, .b = 0x00};

uint8_t current_time[4] = {0, 0, 0, 0};
uint8_t _leds_in_segment;

void clock_set_symbol(led_clock_t *clock, uint8_t symbol, uint8_t position)
{
    if (symbol > 10)
    {
        ESP_LOGI("LED_CLOCK", "Wrong symbol");
        return;
    }

    uint8_t newBytes = SYMBOLS[symbol];
    // uint8_t diff = newBytes ^ current_time[position];
    size_t offset = _leds_in_segment * 7 * position;
    led_color_t active_color = clock->color;

    // for (int i = 0; i < 7; i++)
    // {
    //     if (diff & (1 << i))
    //     {
    //         if (newBytes & (1 << i))
    //         {
    //             for ()
    //             led_set_pixel(clock->strip, active_color, offset + i);
    //             ESP_LOGI("LED_CLOCK", "i = %d, color = %u", offset + i, active_color);
    //         }
    //         else
    //         {
    //             led_set_pixel(clock->strip, deactive_color, offset + i * _leds_in_segment);
    //             ESP_LOGI("LED_CLOCK", "i = %d, color = %u", offset + i, deactive_color);
    //         }
    //     }
    // }
    // current_time[position] = newBytes;

    for (int i = 0; i < clock->leds_in_segment * 7; i++)
    {
        if (newBytes & (1 << i / clock->leds_in_segment))
        {
            // ESP_LOGI("LED_CLOCK", "i = %d, color = %u", offset + i, active_color);
            led_set_pixel(clock->strip, active_color, offset + i);
        }
        else
        {
            // ESP_LOGI("LED_CLOCK", "i = %d, color = %u", offset + i, deactive_color);
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
    _leds_in_segment = leds_in_segment;
    clock->color = WHITE;
    clock->brightness = 0xA0;

    // led_set_color(strip, clock->color);
    led_set_brightness(strip, clock->brightness);
    led_clear(clock->strip);
    return ESP_OK;
}

void clock_set_color(led_clock_t *clock, uint32_t color) {};
void clock_set_brightness(led_clock_t *clock, uint8_t brightness) {};
void clock_set_time(led_clock_t *clock, time_t time)
{
    struct tm *now = localtime(&time);
    ESP_LOGI("LED_CLOCK", "new time %lld", now);
    if (now->tm_min / 10 != current_time[0])
    {
        clock_set_symbol(clock, now->tm_min / 10, 0);
        current_time[0] = now->tm_min / 10;
    };

    if (now->tm_min % 10 != current_time[1])
    {
        clock_set_symbol(clock, now->tm_min % 10, 1);
        current_time[1] = now->tm_min % 10;
    };

    if (now->tm_sec / 10 != current_time[2])
    {
        clock_set_symbol(clock, now->tm_sec / 10, 2);
        current_time[2] = now->tm_sec / 10;
    };

    if (now->tm_sec % 10 != current_time[3])
    {
        clock_set_symbol(clock, now->tm_sec % 10, 3);
        current_time[3] = now->tm_sec % 10;
    };

    // clock_set_symbol(clock, now->tm_min % 10, 1);
    // clock_set_symbol(clock, now->tm_sec / 10, 2);
    // clock_set_symbol(clock, now->tm_sec % 10, 3);
    led_show(clock->strip);
};

// void setSymbol(led_color_t color, int sym){
//     ESP_LOGI(TAG, "New symbol %d", sym);
//     if (sym < 0 || sym > 10){
//         ESP_LOGI(TAG, "Wrong symbol");
//         return;
//     }
//     led_color_t new_color;
//     uint8_t symBytes = SYMBOLS[sym];
//     for (int i = 0; i < 8; i++){

//         if (symBytes & (1 << i)) {
//             new_color = color;
//         }
//         else{
//             new_color.r = 0;
//             new_color.g = 0;
//             new_color.b = 0;
//         }

//         for (int j = 0; j < LEDS_IN_SEGMENT; j++){
//                 int led_num = i * LEDS_IN_SEGMENT + j;
//                 LEDS[led_num * 3] = new_color.g;
//                 LEDS[led_num * 3 + 1] = new_color.r;
//                 LEDS[led_num * 3 + 2] = new_color.b;
//             }
//     }
//     updateLeds();
// }